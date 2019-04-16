package com.jimi.adct_server.adct.handle;

import java.util.Date;
import java.util.Iterator;

import javax.websocket.Session;

import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

import com.alibaba.fastjson.JSON;
import com.alibaba.fastjson.JSONException;
import com.jimi.adct_server.adct.container.RequestBox;
import com.jimi.adct_server.adct.container.SessionBox;
import com.jimi.adct_server.adct.controller.RequestDispatcher;
import com.jimi.adct_server.adct.entity.PastaPackage;
import com.jimi.adct_server.adct.entity.RequestResponsePair;
import com.jimi.adct_server.comm.constant.MyConst;
import com.jimi.adct_server.comm.exception.ParameterException;
import com.jimi.adct_server.comm.util.ErrorLogger;

/**
 * 报文接受者
 * <br>
 * <b>2019年4月9日</b>
 * @author 几米物联自动化部-洪达浩
 */
public class PackageReceiver {

	private static final Logger logger = LogManager.getRootLogger();
	
	
	public static void recevie(Session session, String message) {
		try {
			try {
				//解析报文数据
				PastaPackage pastaPackage = JSON.parseObject(message, PastaPackage.class);
				//方向判断
				String direction = pastaPackage.getDirection();
				if(direction.equals(PastaPackage.REQUEST)) {
					handleRequest(session, pastaPackage);
				}else if(direction.equals(PastaPackage.RESPONSE)){
					handleResponse(session, pastaPackage);
				}else {
					throw new ParameterException("接收到一则存在非法方向的报文："+direction); 
				}
			} catch (JSONException e) {
				throw new ParameterException("无法解析JSON：" + message); 
			}
		} catch (Exception e) {
			ErrorLogger.logError(logger, e);
		}
	}

	
	private static void handleResponse(Session session, PastaPackage responsePackage) {
		int responseId = responsePackage.getId();
		Integer sessionId = SessionBox.getIdBySession(session);
		synchronized (RequestBox.class) {
			//遍历RequestResponsePairSet中是否存在满足ID为id并且session为ws的元素？
			Iterator<RequestResponsePair> it = RequestBox.getReference().iterator();
			while (it.hasNext()) {
				RequestResponsePair pair = it.next();
				int idInBox = pair.getRequestPackage().getId();
				int toIdInBox = pair.getToId();
				if (idInBox == responseId && sessionId.intValue() == toIdInBox) {
					//记录WS日志
					PackageLogger.log(pair);
					//移除元素
					it.remove();
					//设置响应
					pair.setResponsePackage(responsePackage);
					synchronized (pair) {
						//唤醒线程
						pair.notify();
					}
					return;
				}
			}
		}
	}

	
	private static void handleRequest(Session session, PastaPackage requestPackage) {
		Date requestTime = new Date();
		//分发请求，下达业务控制层进行实际处理，返回响应报文
		PastaPackage responsePackage = RequestDispatcher.dispatch(session, requestPackage);
		//发送响应报文
		PackageSender.sendResponse(session, responsePackage);
		//生成请求响应对
		Integer sessionId = SessionBox.getIdBySession(session);
		RequestResponsePair pair = new RequestResponsePair(requestTime, requestPackage, sessionId, MyConst.SERVER_WS_ID);
		//设置响应报文到请求响应对中
		pair.setResponsePackage(responsePackage);
		//记录WS日志
		PackageLogger.log(pair);
	}
	
}
