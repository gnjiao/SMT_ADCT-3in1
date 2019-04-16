package com.jimi.adct_server.front.controller;

import java.util.Date;

import com.jfinal.aop.Enhancer;
import com.jfinal.core.Controller;
import com.jimi.adct_server.comm.service.LogService;
import com.jimi.adct_server.comm.util.ResultFactory;
import com.jimi.adct_server.front.annotation.Log;

/**
 * CYT报表控制器
 * <br>
 * <b>2019年3月28日</b>
 * @author 几米物联自动化部-洪达浩
 */
public class ReportController extends Controller {

	private static LogService logService = Enhancer.enhance(LogService.class);

	
	@Log("查询从{startTime}到{endTime}的产能")
	public void showCapacity(Date startTime, Date endTime, Integer continueById, Integer reviewById) {
		renderJson(ResultFactory.succeed(logService.statisticsSucceedAndFailedLog(startTime, endTime, continueById, reviewById)));
	}
	
}
