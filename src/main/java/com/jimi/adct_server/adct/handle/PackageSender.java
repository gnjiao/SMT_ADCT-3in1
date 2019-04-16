package com.jimi.adct_server.adct.handle;

import java.io.IOException;
import java.util.Date;
import java.util.Map;

import javax.websocket.Session;

import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

import com.alibaba.fastjson.JSON;
import com.jimi.adct_server.adct.container.RequestBox;
import com.jimi.adct_server.adct.container.SessionBox;
import com.jimi.adct_server.adct.entity.PastaPackage;
import com.jimi.adct_server.adct.entity.RequestResponsePair;
import com.jimi.adct_server.comm.constant.MyConst;
import com.jimi.adct_server.comm.exception.ExceptionManager;
import com.jimi.adct_server.comm.exception.ParameterException;
import com.jimi.adct_server.comm.util.ErrorLogger;
import com.jimi.adct_server.comm.util.ResultFactory.Result;

/**
 * 报文发送者
 * <br>
 * <b>2019年4月9日</b>
 * @author 几米物联自动化部-洪达浩
 */
public class PackageSender {

	private static final Logger logger = LogManager.getRootLogger();
	
	
	/**
	 * 发送报文，阻塞等待响应结果【底层框架的起点】
	 * @param sessionIdInBox
	 * @param requestType
	 * @param bodyParameters
	 * @return
	 * @throws Exception 抛出异常给表层处理
	 */
	public static Result sendRequest(Integer sessionIdInBox, String requestType, Map<String, Object> bodyParameters) throws Exception {
		//判断sessionID是否非空并且有对应session
		if(sessionIdInBox == null) {
			throw new ParameterException("实例ID不能为空");
		}
		Session session = SessionBox.getSessionById(sessionIdInBox);
		if(session == null) {
			throw new ParameterException("该ID的实例不在线或者不存在");
		}
		//把请求报文加入到RequestBox中
		PastaPackage requestPackage = new PastaPackage(requestType, bodyParameters);
		RequestResponsePair pair = new RequestResponsePair(new Date(), requestPackage, MyConst.SERVER_WS_ID, sessionIdInBox);
		synchronized (RequestBox.class) {
			RequestBox.getReference().add(pair);
		}
		//发送请求报文数据
		session.getBasicRemote().sendText(JSON.toJSONString(requestPackage));
		//等待响应通知或超时通知
		synchronized (pair) {
			pair.wait();
		}
		//返回结果
		PastaPackage responsePackage = pair.getResponsePackage();
		Result result = JSON.parseObject(responsePackage.getBody().toString(), Result.class);
		//判断是否需要抛异常
		if(result.getCode() == 200) {
			return result;
		}else {
			//如果异常码是5XX，在此转换成300返回给前端
			if(result.getCode().toString().startsWith("5")) {
				result.setCode(300);
			}
			//如果异常信息是字符串型则直接抛出异常，否则，把异常信息用一句常量字符串代替再抛出
			if(result.getData() instanceof String) {
				throw ExceptionManager.getException(result.getCode(), (String)result.getData());
			}else {
				throw ExceptionManager.getException(result.getCode(), "#出错详情无法解析成字符串#");
			}
		}
	}

	
	static void sendResponse(Session session, PastaPackage responsePackage) {
		//发送响应报文数据
		try {
			String message = JSON.toJSONString(responsePackage);
			logger.debug("session发送信息，ID为:"+session.getId()+"，信息为:"+message);
			session.getBasicRemote().sendText(message);
		} catch (IOException e) {
			ErrorLogger.logError(logger, e);
		}
	}
	
}
