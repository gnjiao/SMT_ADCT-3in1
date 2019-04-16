import Vue from 'vue'
import Vuex from 'vuex'
import * as mutations from './mutations'
import * as actions from './actions'
import * as getters from './getters'

Vue.use(Vuex);

const state = {
    //token标识
    token: '',

    //instance页面 点击上一页和下一页之前保留的页面信息
    page:{
        continueById:'',
        reviewById:''
    }
};

const store = new Vuex.Store({
    state,
    getters,
    actions,
    mutations
});

export default store;
