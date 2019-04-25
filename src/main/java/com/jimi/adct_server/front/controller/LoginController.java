package com.jimi.adct_server.front.controller;

import java.util.HashMap;
import java.util.Map;

import com.jfinal.aop.Enhancer;
import com.jfinal.core.Controller;
import com.jfinal.plugin.activerecord.Record;
import com.jimi.adct_server.comm.constant.MyConst;
import com.jimi.adct_server.comm.exception.OperationException;
import com.jimi.adct_server.comm.exception.ParameterException;
import com.jimi.adct_server.comm.service.UserService;
import com.jimi.adct_server.comm.util.ResultFactory;
import com.jimi.adct_server.front.annotation.Log;
import com.jimi.adct_server.front.util.TokenBox;

/**
 * 登录控制器
 * <br>
 * <b>2019年3月28日</b>
 * @author 几米物联自动化部-洪达浩
 */
public class LoginController extends Controller {

	private static UserService userService = Enhancer.enhance(UserService.class);

	
	@Log("登录系统，用户密码为{uid}, {password}")
	public void login(String uid, String password) {
		//判断是否重复登录
		String tokenId = getPara(TokenBox.TOKEN_ID_KEY_NAME);
		Record userInDb = userService.getUserByIdAndPassword(uid, password);
		if(userInDb == null) {
			throw new OperationException("用户名或密码错误");
		}
		if(tokenId != null) {
			Record userInSession = TokenBox.get(tokenId, MyConst.LOGIN_USER);
			if(userInSession != null && userInDb.get("UserId").equals(userInSession.get("UserId"))) {
				throw new ParameterException("请勿重复登录！");
			}
		}
		String newTokenId = TokenBox.createTokenId();
		TokenBox.put(newTokenId, MyConst.LOGIN_USER, userInDb);
		Map<String,Object> resultMap = new HashMap<>();
		resultMap.put("#TOKEN#", newTokenId);
		resultMap.put("name", userInDb.get("UserDes"));
		renderJson(ResultFactory.succeed(resultMap));
	}
	
}
