package com.jimi.adct_server.adct.timer;

import java.util.Date;
import java.util.Iterator;

import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

import com.jimi.adct_server.adct.container.RequestBox;
import com.jimi.adct_server.adct.entity.PastaPackage;
import com.jimi.adct_server.adct.entity.RequestResponsePair;
import com.jimi.adct_server.adct.handle.PackageLogger;
import com.jimi.adct_server.comm.constant.ResultCode;
import com.jimi.adct_server.comm.util.ErrorLogger;
import com.jimi.adct_server.comm.util.ResultFactory;
import com.jimi.adct_server.comm.util.ResultFactory.Result;

/**
 * 超时请求检测定时器
 * <br>
 * <b>2019年4月9日</b>
 * @author 几米物联自动化部-洪达浩
 */
public class RequestTimeoutCheckTimer{

	private static final Logger logger = LogManager.getRootLogger();
	
	private static final Thread thread = new Thread() {
		
		@Override
		public void run() {
			try {
				while (true) {
					try {
						handleTimeout();
					} catch (Exception e) {
						ErrorLogger.logError(logger, e);
						sleep(30 * 1000);//30秒后再次运行循环体
					}
					sleep(1000);//周期为1秒
				} 
			}catch (InterruptedException e) {
				logger.debug("超时检测线程正常中断");
			}
		}

		
		private void handleTimeout() {
			synchronized (RequestBox.class) {
				//遍历RequestResponsePairSet中是否存在超时的请求？（根据类型-超时表判断）
				Iterator<RequestResponsePair> it = RequestBox.getReference().iterator();
				while (it.hasNext()) {
					RequestResponsePair pair = it.next();
					String typeName = pair.getRequestPackage().getType();
					long timeoutTime = RequestTypeTimeoutTimeManager.getTimeoutTime(typeName);
					long requestTime = pair.getRequestTime().getTime();
					long nowTime = new Date().getTime();
					if (nowTime - requestTime > timeoutTime) {
						//记录WS日志
						PackageLogger.logTimeout(pair);
						//移除元素
						it.remove();
						//设置响应
						Result responseBody = ResultFactory.failed(ResultCode.REQUEST_TIMEOUT_EXCEPTION_CODE, "请求实例超时");
						PastaPackage responsePackage = new PastaPackage(pair.getRequestPackage(), responseBody);
						pair.setResponsePackage(responsePackage);
						synchronized (pair) {
							//唤醒线程
							pair.notify();
						}
					}
				}
			}
		}
		
	};
	
	
	public static void start() {
		thread.setName("request-timeout-check-timer");
		thread.start();
	}
	
	
	public static void stop() {
		thread.interrupt();
	}
	
}
