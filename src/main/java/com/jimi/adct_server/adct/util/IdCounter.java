package com.jimi.adct_server.adct.util;

/**
 * 报文ID计数器
 * <br>
 * <b>2019年4月9日</b>
 * @author 几米物联自动化部-洪达浩
 */
public class IdCounter {

	private static int id = 0;
	
	public static synchronized int inc() {
		return id++;
	}
	
}
