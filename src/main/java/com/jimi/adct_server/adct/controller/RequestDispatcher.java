package com.jimi.adct_server.adct.controller;

import javax.websocket.Session;

import com.alibaba.fastjson.JSONObject;
import com.jimi.adct_server.adct.entity.PastaPackage;
import com.jimi.adct_server.comm.constant.ADCTPastaType;
import com.jimi.adct_server.comm.exception.DaException;
import com.jimi.adct_server.comm.exception.ExceptionManager;
import com.jimi.adct_server.comm.exception.ParameterException;
import com.jimi.adct_server.comm.util.ResultFactory;
import com.jimi.adct_server.comm.util.ResultFactory.Result;

/**
 * 请求调度者【具体业务的起点】
 * <br>
 * <b>2019年4月9日</b>
 * @author 几米物联自动化部-洪达浩
 */
public class RequestDispatcher {

	public static PastaPackage dispatch(Session session, PastaPackage requestPackage) {
		String type = requestPackage.getType();
		JSONObject requestBody = (JSONObject) requestPackage.getBody();
		Result responseBody = null;
		try {
			switch (type) {
			case ADCTPastaType.LOGIN:
				responseBody = LoginController.login(session, requestBody);
				break;
			case ADCTPastaType.LOG:
				responseBody = LogController.log(session, requestBody);
				break;
			default:
				throw new ParameterException("不支持的请求类型："+type);
			}
		} catch (DaException e) {
			//捕获到DaException的子类就生成出错响应报文体
			responseBody = ResultFactory.failed(ExceptionManager.getResultCode(e), e.getMessage());
		}
		return new PastaPackage(requestPackage, responseBody);
	}
	
}
