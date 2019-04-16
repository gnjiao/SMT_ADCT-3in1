package com.jimi.adct_server.front.controller;

import com.jfinal.aop.Enhancer;
import com.jfinal.core.Controller;
import com.jimi.adct_server.comm.service.InstanceService;
import com.jimi.adct_server.comm.util.ResultFactory;
import com.jimi.adct_server.front.annotation.Log;

/**
 * CYT报表控制器
 * <br>
 * <b>2019年3月28日</b>
 * @author 几米物联自动化部-洪达浩
 */
public class InstanceController extends Controller {

	private static InstanceService instanceService = Enhancer.enhance(InstanceService.class);

	
	@Log("搜索并展示名字为{instanceName}的实例")
	public void showInstances(String instanceNameKey, Integer continueById, Integer reviewById) {
		renderJson(ResultFactory.succeed(instanceService.searchInstanceByName(instanceNameKey, continueById, reviewById)));
	}
	
	
	@Log("删除ID为{id}的实例")
	public void deleteInstance(Integer id) {
		renderJson(ResultFactory.succeed(instanceService.deleteInstance(id)));
	}
	
}
