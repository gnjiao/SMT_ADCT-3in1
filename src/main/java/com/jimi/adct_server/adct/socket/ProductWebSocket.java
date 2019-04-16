package com.jimi.adct_server.adct.socket;

import javax.websocket.CloseReason;
import javax.websocket.OnClose;
import javax.websocket.OnError;
import javax.websocket.OnMessage;
import javax.websocket.OnOpen;
import javax.websocket.Session;
import javax.websocket.server.ServerEndpoint;

import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

import com.jimi.adct_server.adct.container.SessionBox;
import com.jimi.adct_server.adct.handle.PackageReceiver;
import com.jimi.adct_server.comm.util.ErrorLogger;


/**
 * 生产环境数据通讯websocket
 * <br>
 * <b>2019年3月6日</b>
 * @author 几米物联自动化部-洪达浩
 */
@ServerEndpoint("/product")
public class ProductWebSocket {

	private static final Logger logger = LogManager.getRootLogger();


	@OnOpen
	public void onOpen(Session session) {
		logger.debug("session接入，ID为:"+session.getId());
	}

	
	@OnMessage
	public void onMessage(Session session, String message) {
		logger.debug("session收到信息，ID为:"+session.getId()+"，信息为:"+message);
		PackageReceiver.recevie(session, message);
	}


	@OnClose
	public void onClose(Session session, CloseReason closeReason) {
		logger.warn("session关闭，ID为:"+session.getId()+"，原因为:"+closeReason.getReasonPhrase());
		SessionBox.removeSession(session);
	}


	@OnError
	public void onError(Session session, Throwable error) {
		if(error instanceof Exception) {
			ErrorLogger.logError(logger, (Exception) error);
		}else {
			logger.warn("session发生错误，ID为:"+session.getId()+"，信息为:"+error.getMessage());
		}
		
	}
	
}
