<template>
    <div class="header">
        <div class="header-title">
            <span class="title-icon" @click="collapse"><i class="el-icon-coke-fold"></i></span>
            <span class="title-name">ADCT三合一测试</span>
        </div>
        <div class="header-return">
            <el-button type="warning" @click="logout">退出</el-button>
        </div>
    </div>
</template>

<script>
    import {mapActions} from 'vuex'
    import Bus from './../utils/bus.js'
    import {logoutUrl} from "../plugins/globalUrl";
    import {axiosPost} from "../utils/fetchData";
    import {errHandler} from "../utils/errorHandler";
    export default {
        name: "PageHeader",
        data(){
            return{
                isPending:false
            }
        },
        methods:{
            ...mapActions(['setToken']),
            collapse:function(){
                Bus.$emit('collapse',true);
            },
            logout:function(){
                if (!this.isPending) {
                    this.isPending = true;
                    let options = {
                        url: logoutUrl,
                        data: {
                        }
                    };
                    axiosPost(options).then(response => {
                        this.isPending = false;
                        if (response.data.code === 200) {
                            this.$alertSuccess('登出成功');
                            this.setToken('');
                            localStorage.removeItem('token');
                            this.$router.replace('/login')
                        } else {
                            errHandler(response.data);
                        }
                    })
                        .catch(err => {
                            this.$alertError('请求超时，请刷新重试');
                            console.log(err);
                            this.isPending = false;
                        })
                }
            }
        }
    }
</script>

<style scoped lang="scss">
    .header {
        display: flex;
        justify-content: space-between;
        align-items: center;
        width: 100%;
        height: 100%;
        color:#fff;
        .header-title {
            .title-icon{
                display:inline-block;
                width:44px;
                cursor:pointer;
                i{
                    font-size:22px;
                }
            }
            .title-name{
                display: inline-block;
                font-size:22px;
                font-weight:bold;
            }
        }
        .header-return{
            display: flex;
            align-content: center;
        }
    }
</style>