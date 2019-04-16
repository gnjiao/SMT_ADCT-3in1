package com.jimi.adct_server.comm.util;

import com.alibaba.fastjson.JSON;
import com.jimi.adct_server.comm.constant.ResultCode;

/**
 * 结果工厂<br>
 * 创造带result字段和data字段的结果实体
 */
public class ResultFactory {
	
	public static class Result {
		
		private Integer code;
		
		private Object data;

		
		public Integer getCode() {
			return code;
		}


		public void setCode(Integer code) {
			this.code = code;
		}


		public Object getData() {
			return data;
		}


		public void setData(Object data) {
			this.data = data;
		}
		
		
		@Override
		public String toString() {
			return JSON.toJSONString(this);
		}
		
	}

	
	public static Result succeed() {
		return succeed("操作成功");
	}
	
	
	public static Result succeed(Object data) {
		Result result = new Result();
		result.code = ResultCode.SUCCESS_CODE;
		result.data = data;
		return result;
	}
	
	
	public static Result failed(int code, Object errorMsg) {
		Result result = new Result();
		result.code = code;
		result.data = errorMsg;
		return result;
	}
	
}
