package com.jimi.adct_server.adct.entity;

import java.util.Date;

/**
 * 请求-响应对
 * <br>
 * <b>2019年4月9日</b>
 * @author 几米物联自动化部-洪达浩
 */
public class RequestResponsePair {

	private Date requestTime;
	
	private PastaPackage requestPackage;
	
	private PastaPackage responsePackage;
	
	private Integer fromId;
	
	private Integer toId;

	
	public RequestResponsePair(Date requestTime, PastaPackage requestPackage, Integer fromId, Integer toId) {
		this.requestTime = requestTime;
		this.requestPackage = requestPackage;
		this.fromId = fromId;
		this.toId = toId;
	}

	public Date getRequestTime() {
		return requestTime;
	}

	public PastaPackage getRequestPackage() {
		return requestPackage;
	}

	public PastaPackage getResponsePackage() {
		return responsePackage;
	}

	public Integer getFromId() {
		return fromId;
	}

	public Integer getToId() {
		return toId;
	}

	public void setResponsePackage(PastaPackage responsePackage) {
		this.responsePackage = responsePackage;
	}
	
}
