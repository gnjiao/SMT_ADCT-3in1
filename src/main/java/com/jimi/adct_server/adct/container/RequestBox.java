package com.jimi.adct_server.adct.container;

import java.util.HashSet;
import java.util.Set;

import com.jimi.adct_server.adct.entity.RequestResponsePair;

/**
 * 存放请求的容器
 * <br>
 * <b>2019年4月9日</b>
 * @author 几米物联自动化部-洪达浩
 */
public class RequestBox {
	
	public static final Set<RequestResponsePair> requestResponsePairSet = new HashSet<>();
	
	public static Set<RequestResponsePair> getReference() {
		return requestResponsePairSet;
	}
	
}
