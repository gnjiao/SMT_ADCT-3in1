package com.jimi.adct_server.front.controller;

import javax.websocket.Session;

import com.alibaba.fastjson.JSONObject;
import com.jfinal.core.Controller;
import com.jimi.adct_server.adct.container.SessionBox;
import com.jimi.adct_server.comm.constant.ADCTPastaType;
import com.jimi.adct_server.comm.exception.ExceptionManager;
import com.jimi.adct_server.comm.exception.ParameterException;
import com.jimi.adct_server.front.annotation.Log;

import cc.darhao.pasta.Pasta;

/**
 * 控制控制器
 * <br>
 * <b>2019年3月28日</b>
 * @author 几米物联自动化部-洪达浩
 */
public class ControlController extends Controller {

	@Log("获取实例ID为{instanceId}的实例的工作状态")
	public void getState(Integer instanceId) throws Exception {
		if(instanceId == null) {
			throw new ParameterException("实例ID不能为空");
		}
		Session session = SessionBox.getSessionById(instanceId);
		if(session == null) {
			throw new ParameterException("实例ID不在线或不存在");
		}
		JSONObject result = Pasta.sendRequest(session, ADCTPastaType.GET_STATE, null);
		throwExceptionIfExistByResult(result);
		renderJson(result);
	}
	
	
	@Log("模糊查询实例ID为{instanceId}、订单关键字为{orderKey}的订单")
	public void searchOrders(Integer instanceId, String orderKey) throws Exception {
		if(instanceId == null || orderKey == null) {
			throw new ParameterException("订单关键字和实例ID不能为空");
		}
		Session session = SessionBox.getSessionById(instanceId);
		if(session == null) {
			throw new ParameterException("实例ID不在线或不存在");
		}
		JSONObject parametes = new JSONObject();
		parametes.put("key", orderKey);
		JSONObject result = Pasta.sendRequest(session, ADCTPastaType.GET_ORDERS, parametes);
		throwExceptionIfExistByResult(result);
		renderJson(result);
	}
	
	
	@Log("将订单号为{order}、版本为{version}的订单绑定到实例ID为{instanceId}的实例中")
	public void bind(Integer instanceId, String order, String version) throws Exception {
		if(instanceId == null || order == null || version == null) {
			throw new ParameterException("实例ID、订单号和版本号不能为空");
		}
		Session session = SessionBox.getSessionById(instanceId);
		if(session == null) {
			throw new ParameterException("实例ID不在线或不存在");
		}
		JSONObject parametes = new JSONObject();
		parametes.put("order", order);
		parametes.put("version", version);
		JSONObject result = Pasta.sendRequest(session, ADCTPastaType.BIND, parametes);
		throwExceptionIfExistByResult(result);
		renderJson(result);
	}
	
	
	@Log("令实例ID为{instanceId}的实例开始工作")
	public void start(Integer instanceId) throws Exception {
		Session session = SessionBox.getSessionById(instanceId);
		if(session == null) {
			throw new ParameterException("实例ID不在线或不存在");
		}
		JSONObject result = Pasta.sendRequest(session, ADCTPastaType.START, null);
		throwExceptionIfExistByResult(result);
		renderJson(result);
	}
	
	
	@Log("令实例ID为{instanceId}的实例停止工作")
	public void stop(Integer instanceId) throws Exception {
		Session session = SessionBox.getSessionById(instanceId);
		if(session == null) {
			throw new ParameterException("实例ID不在线或不存在");
		}
		JSONObject result = Pasta.sendRequest(session, ADCTPastaType.STOP, null);
		throwExceptionIfExistByResult(result);
		renderJson(result);
	}
	
	
	@Log("将实例ID为{instanceId}的实例中订单取消绑定")
	public void cancel(Integer instanceId) throws Exception {
		Session session = SessionBox.getSessionById(instanceId);
		if(session == null) {
			throw new ParameterException("实例ID不在线或不存在");
		}
		JSONObject result = Pasta.sendRequest(session, ADCTPastaType.CANCEL, null);
		throwExceptionIfExistByResult(result);
		renderJson(result);
	}
	
	
	
	private void throwExceptionIfExistByResult(JSONObject result) throws Exception {
		if(result.getInteger("code").toString().equals("200")) {
			return;
		}
		//如果异常码是5XX，在此转换成300返回给前端
		if(result.getInteger("code").toString().startsWith("5")) {
			result.put("code", 300);
		}
		//如果异常信息是字符串型则直接抛出异常，否则，把异常信息用一句常量字符串代替再抛出
		if(result.get("data") instanceof String) {
			throw ExceptionManager.getException(result.getInteger("code"), result.getString("data"));
		}else {
			throw ExceptionManager.getException(result.getInteger("code"), "#出错详情无法解析成字符串#");
		}
	}
	
}
