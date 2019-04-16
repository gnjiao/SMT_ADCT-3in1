package com.jimi.adct_server.comm.service;

import java.util.Collections;
import java.util.HashSet;
import java.util.List;
import java.util.Set;

import com.jfinal.plugin.activerecord.Record;
import com.jimi.adct_server.adct.container.SessionBox;
import com.jimi.adct_server.comm.exception.ParameterException;
import com.jimi.adct_server.comm.exception.UnknownException;
import com.jimi.adct_server.comm.model.Adct;
import com.jimi.adct_server.comm.service.base.SelectService;

public class InstanceService extends SelectService{

	public List<Record> searchInstanceByName(String instanceNameKey, Integer continueById, Integer reviewById) {
		List<Record> results = null;
		if(continueById != null && reviewById != null) {
			throw new ParameterException("reviewById与continueById不能同时有值");
		}
		Set<Expression> expressions = new HashSet<>();
		if(instanceNameKey != null) {
			expressions.add(new Expression("name", Expression.LIKE, instanceNameKey));
		}
		if(continueById != null && reviewById != null) {
			throw new ParameterException("reviewById与continueById不能同时有值");
		}else if(continueById != null) {
			expressions.add(new Expression("id", Expression.GREATER, continueById));
			results = select("mysql","adct", null, 20, null, null, expressions);
		}else if(reviewById != null) {
			expressions.add(new Expression("id", Expression.LESS, reviewById));
			results = select("mysql","adct", null, 20, null, "id", expressions);
			Collections.reverse(results);
		}else {
			results = select("mysql","adct", null, 20, null, null, expressions);
		}
		//增加在线状态字段
		for (Record record : results) {
			if(SessionBox.getSessionById(record.getInt("id")) != null) {
				record.set("online", true);
			}else {
				record.set("online", false);
			}
		}
		return results;
	}

	
	public String deleteInstance(Integer id) {
		if(id == null) {
			throw new ParameterException("ID不能为空");
		}
		if(Adct.dao.findById(id) == null) {
			throw new ParameterException("该id对应的记录不存在");
		}
		if(SessionBox.getSessionById(id) != null) {
			throw new ParameterException("不能删除在线的实例");
		}
		if(Adct.dao.deleteById(id)) {
			return "删除成功";
		}else {
			throw new UnknownException("未知错误");
		}
	}
	
	
	public Long saveInstance(String name) {
		Adct adct = new Adct();
		adct.setName(name);
		if(adct.save()) {
			return adct.getId();
		}else {
			throw new UnknownException("未知错误");
		}
	}
}
