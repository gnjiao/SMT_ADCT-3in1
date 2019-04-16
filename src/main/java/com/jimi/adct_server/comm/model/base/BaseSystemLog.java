package com.jimi.adct_server.comm.model.base;

import com.jfinal.plugin.activerecord.Model;
import com.jfinal.plugin.activerecord.IBean;

/**
 * Generated by JFinal, do not modify this file.
 */
@SuppressWarnings({"serial", "unchecked"})
public abstract class BaseSystemLog<M extends BaseSystemLog<M>> extends Model<M> implements IBean {

	public M setId(java.lang.Long id) {
		set("id", id);
		return (M)this;
	}
	
	public java.lang.Long getId() {
		return getLong("id");
	}

	public M setTime(java.lang.String time) {
		set("time", time);
		return (M)this;
	}
	
	public java.lang.String getTime() {
		return getStr("time");
	}

	public M setLevel(java.lang.String level) {
		set("level", level);
		return (M)this;
	}
	
	public java.lang.String getLevel() {
		return getStr("level");
	}

	public M setThread(java.lang.String thread) {
		set("thread", thread);
		return (M)this;
	}
	
	public java.lang.String getThread() {
		return getStr("thread");
	}

	public M setPosition(java.lang.String position) {
		set("position", position);
		return (M)this;
	}
	
	public java.lang.String getPosition() {
		return getStr("position");
	}

	public M setMessage(java.lang.String message) {
		set("message", message);
		return (M)this;
	}
	
	public java.lang.String getMessage() {
		return getStr("message");
	}

}
