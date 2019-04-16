package com.jimi.adct_server.comm.service;

import java.util.HashSet;
import java.util.List;
import java.util.Set;

import com.jfinal.plugin.activerecord.Record;
import com.jimi.adct_server.comm.exception.ParameterException;
import com.jimi.adct_server.comm.service.base.SelectService;

public class UserService extends SelectService{

	public Record getUserByIdAndPassword(String uid, String password) {
		if(uid == null && password == null) {
			throw new ParameterException("用户名或密码不能为空");
		}
		Set<Expression> expressions = new HashSet<>();
		expressions.add(new Expression("UserName", Expression.EQUAL, uid));
		expressions.add(new Expression("UserPwd", Expression.EQUAL, password));
		List<Record> users = select("mssql","Gps_User", null, null, null, null, expressions);
		if(users.isEmpty()) {
			return null;
		}
		return users.get(0);
	}

}
