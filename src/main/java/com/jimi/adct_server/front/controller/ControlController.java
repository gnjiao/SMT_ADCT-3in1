package com.jimi.adct_server.front.controller;

import java.util.HashMap;
import java.util.Map;

import com.jfinal.core.Controller;
import com.jimi.adct_server.adct.handle.PackageSender;
import com.jimi.adct_server.comm.constant.ADCTPastaType;
import com.jimi.adct_server.comm.exception.ParameterException;
import com.jimi.adct_server.front.annotation.Log;

/**
 * CYT报表控制器
 * <br>
 * <b>2019年3月28日</b>
 * @author 几米物联自动化部-洪达浩
 */
public class ControlController extends Controller {

	@Log("获取实例ID为{instanceId}的实例的工作状态")
	public void getState(Integer instanceId) throws Exception {
		renderJson(PackageSender.sendRequest(instanceId, ADCTPastaType.GET_STATE, null));
	}
	
	
	@Log("模糊查询实例ID为{instanceId}、订单关键字为{orderKey}的订单")
	public void searchOrders(Integer instanceId, String orderKey) throws Exception {
		if(orderKey == null) {
			throw new ParameterException("订单关键字不能为空");
		}
		Map<String, Object> parametes = new HashMap<>();
		parametes.put("key", orderKey);
		renderJson(PackageSender.sendRequest(instanceId, ADCTPastaType.GET_ORDERS, parametes));
	}
	
	
	@Log("将订单号为{order}、版本为{version}的订单绑定到实例ID为{instanceId}的实例中")
	public void bind(Integer instanceId, String order, String version) throws Exception {
		if(order == null || version == null) {
			throw new ParameterException("订单号和版本号不能为空");
		}
		Map<String, Object> parametes = new HashMap<>();
		parametes.put("order", order);
		parametes.put("version", version);
		renderJson(PackageSender.sendRequest(instanceId, ADCTPastaType.BIND, parametes));
	}
	
	
	@Log("令实例ID为{instanceId}的实例开始工作")
	public void start(Integer instanceId) throws Exception {
		renderJson(PackageSender.sendRequest(instanceId, ADCTPastaType.START, null));
	}
	
	
	@Log("令实例ID为{instanceId}的实例停止工作")
	public void stop(Integer instanceId) throws Exception {
		renderJson(PackageSender.sendRequest(instanceId, ADCTPastaType.STOP, null));
	}
	
	
	@Log("将实例ID为{instanceId}的实例中订单取消绑定")
	public void cancel(Integer instanceId) throws Exception {
		renderJson(PackageSender.sendRequest(instanceId, ADCTPastaType.CANCEL, null));
	}
}
