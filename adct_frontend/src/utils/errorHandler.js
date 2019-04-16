import {alertError,alertWarning} from "./notification"
import router from './../router'
import store from './../store'
export const errHandler = function (data) {
    switch (data.code) {
        case 210:
            alertWarning(data.data);
            break;
        case 211:
            alertWarning(data.data);
            store.commit('setToken', '');
            window.sessionStorage.removeItem('token');
            router.replace({
                path: '/login',
            });
            break;
        case 400:
            alertWarning(data.data);
            break;
        case 500:
            alertError(data.data);
            break;
        case 510:
            alertError(data.data);
            break;
        case 300:
            alertError(data.data);
            break;
        case 310:
            alertError(data.data);
            break;
        default:
            break;
    }
};