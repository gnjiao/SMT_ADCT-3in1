package com.jimi.adct_server.comm.exception;

/**
 * 请求第三方超时异常
 * <br>
 * <b>2018年6月2日</b>
 * @author 沫熊工作室 <a href="http://www.darhao.cc">www.darhao.cc</a>
 */
public class RequestTimeoutException extends DaException {

	public RequestTimeoutException(String message) {
		super(message);
	}
	
}
