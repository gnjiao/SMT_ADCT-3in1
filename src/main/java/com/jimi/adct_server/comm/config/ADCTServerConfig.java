package com.jimi.adct_server.comm.config;

import java.io.File;
import java.sql.Connection;
import java.sql.SQLException;

import org.apache.logging.log4j.Level;
import org.apache.logging.log4j.core.config.Configurator;

import com.jfinal.config.Constants;
import com.jfinal.config.Handlers;
import com.jfinal.config.Interceptors;
import com.jfinal.config.JFinalConfig;
import com.jfinal.config.Plugins;
import com.jfinal.config.Routes;
import com.jfinal.json.MixedJsonFactory;
import com.jfinal.kit.Prop;
import com.jfinal.kit.PropKit;
import com.jfinal.plugin.activerecord.ActiveRecordPlugin;
import com.jfinal.plugin.activerecord.dialect.MysqlDialect;
import com.jfinal.plugin.activerecord.dialect.SqlServerDialect;
import com.jfinal.plugin.activerecord.tx.Tx;
import com.jfinal.plugin.druid.DruidPlugin;
import com.jfinal.template.Engine;
import com.jimi.adct_server.adct.filter.AccessFilter;
import com.jimi.adct_server.adct.filter.ExceptionCatchFilter;
import com.jimi.adct_server.adct.handler.LogHandler;
import com.jimi.adct_server.adct.handler.LoginHandler;
import com.jimi.adct_server.adct.logger.PackageLogger;
import com.jimi.adct_server.comm.constant.RequestTypeTimeoutTimeManager;
import com.jimi.adct_server.comm.handler.WebSocketHandler;
import com.jimi.adct_server.comm.model.MssqlMappingKit;
import com.jimi.adct_server.comm.model.MysqlMappingKit;
import com.jimi.adct_server.front.controller.ControlController;
import com.jimi.adct_server.front.controller.InstanceController;
import com.jimi.adct_server.front.controller.LogController;
import com.jimi.adct_server.front.controller.LoginController;
import com.jimi.adct_server.front.controller.MainController;
import com.jimi.adct_server.front.controller.ReportController;
import com.jimi.adct_server.front.interceptor.AccessInterceptor;
import com.jimi.adct_server.front.interceptor.ActionLogInterceptor;
import com.jimi.adct_server.front.interceptor.CORSInterceptor;
import com.jimi.adct_server.front.interceptor.ErrorLogInterceptor;
import com.jimi.adct_server.front.util.TokenBox;

import cc.darhao.pasta.Pasta;

/**
 * 全局配置
 * <br>
 * <b>2019年4月8日</b>
 * @author 几米物联自动化部-洪达浩
 */
public class ADCTServerConfig extends JFinalConfig {
	
	private static DruidPlugin mysqlDp;
	private static DruidPlugin mssqlDp;
	private Level level;
	
	
	@Override
	public void configConstant(Constants me) {
		me.setJsonFactory(new MixedJsonFactory());
	}

	
	@Override
	public void configEngine(Engine me) {
	}

	
	@Override
	public void configHandler(Handlers me) {
		me.add(new WebSocketHandler("^/product"));
	}

	
	@Override
	public void configInterceptor(Interceptors me) {
		me.addGlobalActionInterceptor(new ErrorLogInterceptor());
		me.addGlobalActionInterceptor(new CORSInterceptor());
		me.addGlobalActionInterceptor(new ActionLogInterceptor());
		me.addGlobalActionInterceptor(new AccessInterceptor());
		me.addGlobalServiceInterceptor(new Tx());
	}

	
	@Override
	public void configPlugin(Plugins me) {
		//设置关闭日志
		Configurator.setRootLevel(Level.OFF);
		Prop prop = PropKit.use("properties.ini");
		//判断环境来设置数据源和日志等级
		level = null;
		mysqlDp = null;
		mssqlDp = null;
		if(isProductionEnvironment()) {
			level = Level.INFO;
			mysqlDp = new DruidPlugin(prop.get("p_url"), prop.get("p_user"), prop.get("p_password"));
			mssqlDp = new DruidPlugin(prop.get("p_url2"), prop.get("p_user2"), prop.get("p_password2"));
			System.out.println("System is in production envrionment");
		} else if(isTestEnvironment()) {
			level = Level.INFO;
			mysqlDp = new DruidPlugin(prop.get("t_url"), prop.get("t_user"), prop.get("t_password"));
			mssqlDp = new DruidPlugin(prop.get("t_url2"), prop.get("t_user2"), prop.get("t_password2"));
			System.out.println("System is in testing envrionment");
		} else {
			level = Level.DEBUG;
			mysqlDp = new DruidPlugin(prop.get("d_url"), prop.get("d_user"), prop.get("d_password"));
			mssqlDp = new DruidPlugin(prop.get("t_url2"), prop.get("t_user2"), prop.get("t_password2"));//mssql我没搭建开发环境
			System.out.println("System is in development envrionment");
		}
		//配置数据库连接池
		me.add(mssqlDp);
		me.add(mysqlDp);
		//配置ORM
	    ActiveRecordPlugin mysqlARP = new ActiveRecordPlugin("mysql",mysqlDp);
	    mysqlARP.setDialect(new MysqlDialect());	// 用什么数据库，就设置什么Dialect
	    mysqlARP.setShowSql(false);
	    ActiveRecordPlugin mssqlARP = new ActiveRecordPlugin("mssql",mssqlDp);
	    mssqlARP.setDialect(new SqlServerDialect());	// 用什么数据库，就设置什么Dialect
	    mssqlARP.setShowSql(false);
	    MysqlMappingKit.mapping(mysqlARP);
	    MssqlMappingKit.mapping(mssqlARP);
	    me.add(mysqlARP);
	    me.add(mssqlARP);
	}

	
	@Override
	public void configRoute(Routes me) {
		me.add("/login", LoginController.class);
		me.add("/main", MainController.class);
		me.add("/main/report", ReportController.class);
		me.add("/main/instance", InstanceController.class);
		me.add("/main/instance/log", LogController.class);
		me.add("/main/instance/control", ControlController.class);
	}

	
	@Override
	public void afterJFinalStart() {
		TokenBox.start(48);
		 //设置日志等级
	    Configurator.setRootLevel(level);
	    //Pasta设置
	    Pasta.bindRoute("login", LoginHandler.class);
	    Pasta.bindRoute("log", LogHandler.class);
	    Pasta.setLogCatcher(new PackageLogger());
	    Pasta.addFilter(new ExceptionCatchFilter());
	    Pasta.addFilter(new AccessFilter());
	    Pasta.startRequestTimeoutChecker(RequestTypeTimeoutTimeManager.getMap());
	}
	
	
	@Override
	public void beforeJFinalStop() {
		TokenBox.stop();
		Pasta.stopRequestTimeoutChecker();
	}
	
	
	public static boolean isProductionEnvironment() {
		File[] roots = File.listRoots();
        for (int i=0; i < roots.length; i++) {
            if(new File(roots[i].toString() + "PRODUCTION_ENVIRONMENT_FLAG").exists()) {
            	return true;
            }
        }
        return false;
	}

	public static boolean isTestEnvironment() {
		File[] roots = File.listRoots();
        for (int i=0; i < roots.length; i++) {
            if(new File(roots[i].toString() + "TEST_ENVIRONMENT_FLAG").exists()) {
            	return true;
            }
        }
        return false;
	}

	
	public static Connection getLog4j2JDBCAppenderConnection() throws SQLException {
		return mysqlDp.getDataSource().getConnection();
	}

}
