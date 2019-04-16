package com.jimi.adct_server.comm.service;

import java.util.Collections;
import java.util.Date;
import java.util.HashSet;
import java.util.List;
import java.util.Set;

import com.jfinal.plugin.activerecord.Db;
import com.jfinal.plugin.activerecord.Record;
import com.jimi.adct_server.comm.exception.ParameterException;
import com.jimi.adct_server.comm.exception.UnknownException;
import com.jimi.adct_server.comm.model.AdctLog;
import com.jimi.adct_server.comm.model.sql.SQL;
import com.jimi.adct_server.comm.service.base.SelectService;

public class LogService extends SelectService{

	public List<Record> getLogByADCT(Integer instanceId, Integer continueById, Integer reviewById) {
		List<Record> results = null;
		Set<Expression> expressions = new HashSet<>();
		if(instanceId == null) {
			throw new ParameterException("InstanceId不能为空");
		}
		expressions.add(new Expression("adct", Expression.EQUAL, instanceId));
		if(continueById != null && reviewById != null) {
			throw new ParameterException("reviewById与continueById不能同时有值");
		}else if(continueById != null) {
			expressions.add(new Expression("id", Expression.GREATER, continueById));
			results = select("mysql","adct_log", null, 20, null, null, expressions);
		}else if(reviewById != null) {
			expressions.add(new Expression("id", Expression.LESS, reviewById));
			results = select("mysql","adct_log", null, 20, null, "id", expressions);
			Collections.reverse(results);
		}else {
			results = select("mysql","adct_log", null, 20, null, null, expressions);
		}
		return results;
	}

	
	public List<Record> statisticsSucceedAndFailedLog(Date startTime, Date endTime, Integer continueById, Integer reviewById) {
		if(startTime.getTime() >= endTime.getTime()) {
			throw new ParameterException("开始时间必须小于结束时间");
		}
		if(continueById != null && reviewById != null) {
			throw new ParameterException("reviewById与continueById不能同时有值");
		}
		if(continueById != null) {
			return Db.use("mysql").find(SQL.STATISTICS_SUCCEED_AND_FAILED_AND_TOTAL_LOG_COUNT_BY_CONTINUE_ID, startTime, endTime, continueById);
		}
		if(reviewById != null) {
			List<Record> result = Db.use("mysql").find(SQL.STATISTICS_SUCCEED_AND_FAILED_AND_TOTAL_LOG_COUNT_BY_REVIEW_ID, startTime, endTime, reviewById);
			Collections.reverse(result);
			return result;
		}
		return Db.use("mysql").find(SQL.STATISTICS_SUCCEED_AND_FAILED_AND_TOTAL_LOG_COUNT, startTime, endTime);
	}
	
	
	public Long saveADCTLog(Integer adctId, String message, Integer type, Date time) {
		AdctLog log = new AdctLog();
		log.setAdct(adctId.longValue());
		log.setMessage(message);
		log.setType(type);
		log.setTime(time);
		if(log.save()) {
			return log.getId();
		}else {
			throw new UnknownException("未知错误");
		}
	}

}
