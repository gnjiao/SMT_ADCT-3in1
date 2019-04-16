package com.jimi.adct_server.front.interceptor;

import com.jfinal.aop.Interceptor;
import com.jfinal.aop.Invocation;
import com.jfinal.plugin.activerecord.Record;
import com.jimi.adct_server.comm.constant.MyConst;
import com.jimi.adct_server.comm.exception.AccessException;
import com.jimi.adct_server.front.util.TokenBox;


/**
 * 权限控制器
 * <b>2019年4月8日</b>
 * @author 沫熊工作室 <a href="http://www.darhao.cc">www.darhao.cc</a>
 */
public class AccessInterceptor implements Interceptor {

	
	@Override
	public void intercept(Invocation invocation) {
		 String token = invocation.getController().getPara(TokenBox.TOKEN_ID_KEY_NAME);
		 if(token == null) {
			//获取方法名
			 String requestMethodName = invocation.getActionKey();
			if(requestMethodName.equals(MyConst.LOGIN_ACTION_KEY)) {
				invocation.invoke();
			}else {
				throw new AccessException("请先登录");
			}
		 }else {
			 Record user = TokenBox.get(token, MyConst.LOGIN_USER);
			 if(user != null) {
				 invocation.invoke();
			 }else {
				 throw new AccessException("请先登录");
			 }
		 }
	}
}
