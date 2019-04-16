package com.jimi.adct_server.adct.timer;

import java.util.HashMap;
import java.util.Map;

import com.jimi.adct_server.comm.constant.ADCTPastaType;

/**
 * 各种请求类型的超时时间
 * <br>
 * <b>2019年4月9日</b>
 * @author 几米物联自动化部-洪达浩
 */
public class RequestTypeTimeoutTimeManager {

	private static final long BIND = 30 * 1000;
	
	private static final long CANCEL = 10 * 1000;
	
	private static final long START = 10 * 1000;
	
	private static final long STOP = 10 * 1000;
	
	private static final long GET_STATE = 10 * 1000;
	
	private static final long GET_ORDERS = 10 * 1000;
	
	private static final Map<String, Long> map = new HashMap<>();
	
	static {
		map.put(ADCTPastaType.BIND, BIND);
		map.put(ADCTPastaType.CANCEL, CANCEL);
		map.put(ADCTPastaType.START, START);
		map.put(ADCTPastaType.STOP, STOP);
		map.put(ADCTPastaType.GET_STATE, GET_STATE);
		map.put(ADCTPastaType.GET_ORDERS, GET_ORDERS);
	}
	
	public static long getTimeoutTime(String type) {
		return map.get(type);
	}
	
}
