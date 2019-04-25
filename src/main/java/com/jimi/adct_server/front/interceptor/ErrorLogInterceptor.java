package com.jimi.adct_server.front.interceptor;

import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

import com.jfinal.aop.Interceptor;
import com.jfinal.aop.Invocation;
import com.jimi.adct_server.comm.exception.ExceptionManager;
import com.jimi.adct_server.comm.util.ErrorLogger;
import com.jimi.adct_server.comm.util.ResultFactory;


/**
 * 错误Logger拦截器
 * <br>
 * <b>2018年5月29日</b>
 * @author 沫熊工作室 <a href="http://www.darhao.cc">www.darhao.cc</a>
 */
public class ErrorLogInterceptor implements Interceptor {

	private static final Logger logger = LogManager.getRootLogger();
	

	@Override
	public void intercept(Invocation invocation) {
		try {
			invocation.invoke();
		}catch (Exception e) {
			ErrorLogger.logError(logger, e);
			invocation.getController().renderJson(ResultFactory.failed(ExceptionManager.getResultCode(e), e.getMessage()));
		}
	}

}