package com.jimi.adct_server.adct.logger;

import java.util.Date;

import com.jimi.adct_server.adct.container.SessionBox;
import com.jimi.adct_server.comm.constant.MyConst;
import com.jimi.adct_server.comm.model.PackageLog;
import cc.darhao.pasta.PastaLogCatcher;
import cc.darhao.pasta.RequestResponsePair;

/**
 * WS日志记录器
 * <br>
 * <b>2019年4月9日</b>
 * @author 几米物联自动化部-洪达浩
 */
public class PackageLogger extends PastaLogCatcher{

	@Override
	public void onRequestFinish(RequestResponsePair info) {
		PackageLog log = createLog(info);
		log.setFromId(MyConst.SERVER_WS_ID);
		log.setToId(SessionBox.getIdBySession(info.getSession()));
		log.setConsumeTime((int) (new Date().getTime() - info.getRequestTime().getTime()));
		log.save();
	}


	@Override
	public void onRequestTimeout(RequestResponsePair info) {
		PackageLog log = createLog(info);
		log.setFromId(MyConst.SERVER_WS_ID);
		log.setToId(SessionBox.getIdBySession(info.getSession()));
		log.setConsumeTime(0);
		log.save();
	}


	@Override
	public void onResponseFinish(RequestResponsePair info) {
		PackageLog log = createLog(info);
		log.setFromId(SessionBox.getIdBySession(info.getSession()));
		log.setToId(MyConst.SERVER_WS_ID);
		log.setConsumeTime((int) (new Date().getTime() - info.getRequestTime().getTime()));
		log.save();
	}


	private static PackageLog createLog(RequestResponsePair pair) {
		PackageLog log = new PackageLog();
		log.setPackageId(pair.getRequestPackage().getId());
		log.setTime(pair.getRequestTime());
		log.setPackageType(pair.getRequestPackage().getType());
		try {
			log.setPackageRequest(pair.getRequestPackage().getBody().toString());
		} catch (NullPointerException e) {
		}
		try {
			log.setPackageResponse(pair.getResponsePackage().getBody().toString());
		} catch (NullPointerException e) {
		}
		return log;
	}
	
}
