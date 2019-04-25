package com.jimi.adct_server.front.controller;

import com.jfinal.core.Controller;
import com.jimi.adct_server.comm.constant.MyConst;
import com.jimi.adct_server.comm.exception.AccessException;
import com.jimi.adct_server.comm.util.ResultFactory;
import com.jimi.adct_server.front.annotation.Log;
import com.jimi.adct_server.front.util.TokenBox;

/**
 * 主控制器
 * <br>
 * <b>2019年3月28日</b>
 * @author 几米物联自动化部-洪达浩
 */
public class MainController extends Controller {

	
	@Log("登出系统")
	public void logout() {
		String tokenId = getPara(TokenBox.TOKEN_ID_KEY_NAME);
		if(TokenBox.get(tokenId, MyConst.LOGIN_USER) == null) {
			throw new AccessException("会话已过期或不存在该id的会话");
		}
		TokenBox.remove(tokenId);
		renderJson(ResultFactory.succeed("登出成功"));
	}
	
}
