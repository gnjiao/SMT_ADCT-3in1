package com.jimi.adct_server.adct.handler;

import java.util.List;

import javax.websocket.Session;

import com.jfinal.aop.Enhancer;
import com.jfinal.plugin.activerecord.Record;
import com.jimi.adct_server.adct.container.SessionBox;
import com.jimi.adct_server.comm.exception.OperationException;
import com.jimi.adct_server.comm.exception.ParameterException;
import com.jimi.adct_server.comm.service.InstanceService;
import com.jimi.adct_server.comm.util.ResultFactory;
import com.jimi.adct_server.comm.util.ResultFactory.Result;

/**
 * 实例端登录控制器
 * <br>
 * <b>2019年4月11日</b>
 * @author 几米物联自动化部-洪达浩
 */
public class LoginHandler {

	private InstanceService instanceService = Enhancer.enhance(InstanceService.class);
	
	
	public Result login(Session session, String name) {
		//如果SessionBox中存在该Session，则提示重复登录
		if(SessionBox.getIdBySession(session) != null) {
			throw new ParameterException("一个连接只能登录一次，请不要重复登录");
		}
		//如果数据库不存在该实例，则保存到数据库并得到ADCT_ID
		List<Record> results = instanceService.searchInstanceByName(name, null, null);
		Long loginAdctId = null;
		if(results.isEmpty()) {
			loginAdctId = instanceService.saveInstance(name);
		}else { //如果数据库存在该实例，则直接获取记录中实例的ID做为ADCT_ID
			loginAdctId = results.get(0).getLong("id");
			//判断SessionBox中是否存在该ID的Session，如果存在，则提示：该名称已经被其他实例登录，如果一定要登录，请先关闭登录了该名称的实例。
			if(SessionBox.getSessionById(loginAdctId.intValue()) != null) {
				throw new OperationException("登录名为： "+name+" 的实例已经被其他实例登录，如果您一定要继续登录，请先关闭登录了该名称的实例");
			}
		}
		//把ID与Session加入到SessionBox中
		SessionBox.addSession(loginAdctId.intValue(), session);
		return ResultFactory.succeed("实例登录成功");
	}
	
}
