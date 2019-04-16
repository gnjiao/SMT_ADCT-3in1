package com.jimi.adct_server.adct.controller;

import java.text.ParseException;
import java.util.Date;

import javax.websocket.Session;

import com.alibaba.fastjson.JSONObject;
import com.jfinal.aop.Enhancer;
import com.jimi.adct_server.adct.container.SessionBox;
import com.jimi.adct_server.comm.constant.ADCTLogType;
import com.jimi.adct_server.comm.exception.AccessException;
import com.jimi.adct_server.comm.exception.ParameterException;
import com.jimi.adct_server.comm.service.LogService;
import com.jimi.adct_server.comm.util.ResultFactory;
import com.jimi.adct_server.comm.util.ResultFactory.Result;

import cc.darhao.dautils.api.DateUtil;

/**
 * 实例端日志控制器
 * <br>
 * <b>2019年4月11日</b>
 * @author 几米物联自动化部-洪达浩
 */
public class LogController {

	private static LogService logService = Enhancer.enhance(LogService.class);
	
	
	public static Result log(Session session, JSONObject requestBody) {
		//判断是否已经登录
		Integer adctId = SessionBox.getIdBySession(session);
		if(adctId == null){
			throw new AccessException("请先登录");
		}
		//记录实例工作日志
		try {
			Date time = DateUtil.yyyyMMddHHmmss(requestBody.getString("time"));
			Integer type = getTypeIdByName(requestBody.getString("type"));
			String message = requestBody.getString("message");
			logService.saveADCTLog(adctId, message, type, time);
			return ResultFactory.succeed("保存日志成功");
		} catch (ParseException e) {
			throw new ParameterException("不符合格式【yyyy-MM-dd HH:mm:ss】的时间字符串："+requestBody.getString("time"));
		}
	}


	private static Integer getTypeIdByName(String typeName) {
		switch (typeName) {
		case ADCTLogType.ERROR_STRING:
			return ADCTLogType.ERROR_CODE;
		case ADCTLogType.WARN_STRING:
			return ADCTLogType.WARN_CODE;
		case ADCTLogType.INFO_STRING:
			return ADCTLogType.INFO_CODE;
		case ADCTLogType.SUCCEED_STRING:
			return ADCTLogType.SUCCEED_CODE;
		case ADCTLogType.FAILED_STRING:
			return ADCTLogType.FAILED_CODE;
		default:
			throw new ParameterException("不存在该日志类型："+typeName);
		}
	}
	
}
