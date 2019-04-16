let url;
if (process.env.NODE_ENV === 'production') {
    url = window.g.API_URL
} else {
    url = window.g.LOCAL_URL
}

//主界面

//主界面-实例界面
let instance = url + '/main/instance';
export const showInstancesUrl = instance + '/showInstances';
export const deleteInstanceUrl = instance + '/deleteInstance';

//主界面-实例界面-控制子界面
let control = instance + '/control';
export const stopUrl = control + '/stop';
export const cancelUrl = control + '/cancel';
export const startUrl = control + '/start';
export const searchOrdersUrl = control + '/searchOrders';
export const bindUrl = control + '/bind';
export const getStateUrl = control + '/getState';

//主界面-实例界面-日志子界面
let log = instance + '/log';
export const showLogsUrl = log + '/showLogs';

//统计界面
let report = url + '/main/report';
export const showCapacityUrl = report + '/showCapacity';

export const logoutUrl = url + '/main/logout';

//登录界面
export const loginUrl = url + '/login/login';