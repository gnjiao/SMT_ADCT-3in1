import Vue from 'vue'
import Router from 'vue-router'
import Main from './views/Main.vue'
import Instance from './views/instance/Instance.vue'
import Report from './views/report/Report.vue'
import Login from './views/login/Login.vue'
import store from './store'

Vue.use(Router);

const router = new Router({
    mode: 'history',
    base: window.g.SYSTEM_PATH,
    routes: [
        {
            path: '/',
            redirect: '/login'
        },
        {
            path: '/main',
            name: 'main',
            component: Main,
            redirect: '/instance',
            meta: {
                requireAuth: true
            },
            children: [
                {
                    path: '/instance',
                    name: 'instance',
                    component: Instance
                },
                {
                    path: '/report',
                    name: 'report',
                    component: Report
                }
            ]
        },
        {
            path: '/login',
            name: 'login',
            component: Login
        }
    ]
});

if (window.sessionStorage.getItem('token')) {
    store.commit('setToken', window.sessionStorage.getItem('token'))
}

router.beforeEach((to, from, next) => {
    if (to.matched.some(r => r.meta.requireAuth)) {
        if (store.state.token) {
            next();
        } else {
            next({
                path: '/login',
            })
        }
    } else {
        next();
    }
});
export default router;
