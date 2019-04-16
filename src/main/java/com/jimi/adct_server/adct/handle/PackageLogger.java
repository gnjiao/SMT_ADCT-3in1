package com.jimi.adct_server.adct.handle;

import java.util.Date;

import com.jimi.adct_server.adct.entity.RequestResponsePair;
import com.jimi.adct_server.comm.model.PackageLog;

/**
 * WS日志记录器
 * <br>
 * <b>2019年4月9日</b>
 * @author 几米物联自动化部-洪达浩
 */
public class PackageLogger {

	public static void log(RequestResponsePair pair) {
		PackageLog log = createLog(pair);
		log.setConsumeTime((int) (new Date().getTime() - pair.getRequestTime().getTime()));
		log.save();
	}

	
	public static void logTimeout(RequestResponsePair pair) {
		PackageLog log = createLog(pair);
		log.setConsumeTime(0);
		log.save();
	}

	
	private static PackageLog createLog(RequestResponsePair pair) {
		PackageLog log = new PackageLog();
		log.setFromId(pair.getFromId());
		log.setToId(pair.getToId());
		log.setPackageId(pair.getRequestPackage().getId());
		log.setTime(pair.getRequestTime());
		log.setPackageType(pair.getRequestPackage().getType());
		try {
			log.setPackageRequest(pair.getRequestPackage().getBody().toString());
		} catch (NullPointerException e) {
		}
		try {
			log.setPackageRequest(pair.getResponsePackage().getBody().toString());
		} catch (NullPointerException e) {
		}
		return log;
	}
	
}
