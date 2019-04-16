package com.jimi.adct_server.adct.entity;

import com.jimi.adct_server.adct.util.IdCounter;

/**
 * Pasta协议报文
 * <br>
 * <b>2019年4月9日</b>
 * @author 几米物联自动化部-洪达浩
 */
public class PastaPackage {
	
	public static final String REQUEST = "request";
	
	public static final String RESPONSE = "response";

	private int id;
	
	private String direction;
	
	private String type;
	
	private Object body;

	
	public PastaPackage() {}
	
	
	/**
	 * 构建请求报文
	 * @param type
	 * @param body
	 */
	public PastaPackage(String requestType, Object requestBody) {
		this.id = IdCounter.inc();
		this.direction = REQUEST;
		this.type = requestType;
		this.body = requestBody;
	}
	
	
	/**
	 * 根据请求报文和响应体生成响应报文
	 * @param requestPackage
	 * @param responseBody
	 */
	public PastaPackage(PastaPackage requestPackage, Object responseBody) {
		this.id = requestPackage.getId();
		this.direction = RESPONSE;
		this.type = requestPackage.getType();
		this.body = responseBody;
	}


	public int getId() {
		return id;
	}


	public String getDirection() {
		return direction;
	}


	public String getType() {
		return type;
	}


	public void setId(int id) {
		this.id = id;
	}


	public void setDirection(String direction) {
		this.direction = direction;
	}


	public void setType(String type) {
		this.type = type;
	}


	public Object getBody() {
		return body;
	}


	public void setBody(Object body) {
		this.body = body;
	}

	
}
