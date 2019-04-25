package com.jimi.adct_server.front.controller;

import java.util.List;

import com.jfinal.aop.Enhancer;
import com.jfinal.core.Controller;
import com.jfinal.plugin.activerecord.Record;
import com.jimi.adct_server.comm.service.LogService;
import com.jimi.adct_server.comm.util.ResultFactory;
import com.jimi.adct_server.front.annotation.Log;

/**
 * 日志控制器
 * <br>
 * <b>2019年3月28日</b>
 * @author 几米物联自动化部-洪达浩
 */
public class LogController extends Controller {

	private static LogService logService = Enhancer.enhance(LogService.class);

	
	@Log("展示实例ID为{instanceId}的日志")
	public void showLogs(Integer instanceId, Integer continueById, Integer reviewById) {
		List<Record> result = logService.getLogByADCT(instanceId, continueById, reviewById);
		for (Record record : result) {
			Integer type = record.get("type");
			switch (type) {
			case 1:
				record.set("type", "error");
				break;
			case 2:
				record.set("type", "warn");
				break;
			case 3:
				record.set("type", "info");
				break;
			case 4:
				record.set("type", "succeed");
				break;
			case 5:
				record.set("type", "failed");
				break;
			default:
				break;
			}
			record.remove("adct");//去掉adct字段
		}
		renderJson(ResultFactory.succeed(result));
	}
	
}
