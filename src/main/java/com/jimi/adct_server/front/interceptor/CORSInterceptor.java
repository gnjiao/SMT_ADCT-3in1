package com.jimi.adct_server.front.interceptor;

import javax.servlet.http.HttpServletResponse;

import com.jfinal.aop.Interceptor;
import com.jfinal.aop.Invocation;


/**
 * 跨域许可拦截器
 * <br>
 * <b>2018年5月29日</b>
 * @author 沫熊工作室 <a href="http://www.darhao.cc">www.darhao.cc</a>
 */
public class CORSInterceptor implements Interceptor {

	@Override
	public void intercept(Invocation invocation) {
		 HttpServletResponse response = invocation.getController().getResponse();
		 response.addHeader("Access-Control-Allow-Origin", "*");
		 invocation.invoke();
	}

}
