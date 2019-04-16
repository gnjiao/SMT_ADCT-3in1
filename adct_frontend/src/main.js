import Vue from 'vue'
import './plugins/axios'
import router from './router'
import store from './store'
import App from './App.vue'
import ElementUI from 'element-ui'
import locale from 'element-ui/lib/locale/lang/zh-CN'
import './styles.scss'
import './assets/icon/iconfont.css'
import {alertError, alertInfo, alertSuccess,alertWarning} from "./utils/notification"
Vue.prototype.$alertError = alertError;
Vue.prototype.$alertInfo = alertInfo;
Vue.prototype.$alertSuccess = alertSuccess;
Vue.prototype.$alertWarning = alertWarning;
Vue.use(ElementUI, { locale });

Vue.config.productionTip = false;
Vue.config.devtools = true;
new Vue({
  router,
  store,
  render: h => h(App)
}).$mount('#app');
