package com.jimi.adct_server.comm.model.sql;

public class SQL {

	//统计成功与失败类型的日志数目
	public static final String STATISTICS_SUCCEED_AND_FAILED_AND_TOTAL_LOG_COUNT = "SELECT adct.`id` AS id, adct.`name` AS name, COUNT(TYPE = 4 OR NULL) AS succeed,COUNT(TYPE = 5 OR NULL) AS failed,COUNT(TYPE IN (4, 5) OR NULL) AS total FROM adct LEFT JOIN (SELECT * FROM adct_log WHERE TIME >= ? AND TIME <= ?) AS tmp ON adct.`id` = tmp.`adct` GROUP BY adct.`id` LIMIT 20";
	
	//统计成功与失败类型的日志数目（统计指定adct的ID后20条）
	public static final String STATISTICS_SUCCEED_AND_FAILED_AND_TOTAL_LOG_COUNT_BY_CONTINUE_ID = "SELECT adct.`id` AS id, adct.`name` AS name, COUNT(TYPE = 4 OR NULL) AS succeed,COUNT(TYPE = 5 OR NULL) AS failed,COUNT(TYPE IN (4, 5) OR NULL) AS total FROM adct LEFT JOIN (SELECT * FROM adct_log WHERE TIME >= ? AND TIME <= ?) AS tmp ON adct.`id` = tmp.`adct`  WHERE adct.`id` > ? GROUP BY adct.`id` LIMIT 20";
	
	//统计成功与失败类型的日志数目（统计指定adct的ID前20条）
	public static final String STATISTICS_SUCCEED_AND_FAILED_AND_TOTAL_LOG_COUNT_BY_REVIEW_ID = "SELECT adct.`id` AS id, adct.`name` AS name, COUNT(TYPE = 4 OR NULL) AS succeed,COUNT(TYPE = 5 OR NULL) AS failed,COUNT(TYPE IN (4, 5) OR NULL) AS total FROM adct LEFT JOIN (SELECT * FROM adct_log WHERE TIME >= ? AND TIME <= ?) AS tmp ON adct.`id` = tmp.`adct`  WHERE adct.`id` < ? GROUP BY adct.`id` ORDER BY adct.`id` DESC LIMIT 20";
	
	
	
}
