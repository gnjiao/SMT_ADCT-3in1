<template>
    <div class="login">
        <div class="login-box">
            <div class="login-name">ADCT三合一测试</div>
            <el-form ref="loginForm" :model="loginInfo" class="login-form" :rules="rules" status-icon>
                <el-form-item label="用户名" prop="uid" >
                    <el-input v-model.trim="loginInfo.uid"  placeholder="用户名" clearable autocomplete="false" spellcheck="false"></el-input>
                </el-form-item>
                <el-form-item label="密码" prop="password">
                    <el-input v-model.trim="loginInfo.password" placeholder="密码" type="password" clearable show-password autocomplete="false" spellcheck="false"></el-input>
                </el-form-item>
                <el-form-item>
                    <el-button type="primary" @click="login('loginForm')" style="width:100%;">登录</el-button>
                </el-form-item>
            </el-form>
        </div>
    </div>
</template>

<script>
    import {mapActions} from 'vuex';
    import {axiosPost} from "../../utils/fetchData";
    import {loginUrl} from "../../plugins/globalUrl";
    import {errHandler} from "../../utils/errorHandler";

    export default {
        name: "Login",
        data(){
            return{
                loginInfo:{
                    uid:'',
                    password:''
                },
                rules: {
                    uid: [
                        { required: true, message: '请输入用户名', trigger: 'change' },
                    ],
                    password: [
                        { required: true, message: '请输入密码', trigger: 'change' }
                    ]
                },
                isPending:false
            }
        },
        methods:{
            ...mapActions(['setToken']),
            login:function (loginForm) {
                this.$refs[loginForm].validate((valid) => {
                    if (valid) {
                        if(!this.isPending){
                            this.isPending = true;
                            let options = {
                                url:loginUrl,
                                data:{
                                    uid:this.loginInfo.uid,
                                    password:this.loginInfo.password
                                }
                            };
                            axiosPost(options).then(response => {
                                this.isPending = false;
                                if(response.data.code === 200){
                                    this.$alertSuccess("登录成功");
                                    let data = response.data.data;
                                    this.setToken(data['#TOKEN#']);
                                    window.sessionStorage.setItem("token",data['#TOKEN#']);
                                    this.$router.replace('/instance');
                                }else{
                                    errHandler(response.data);
                                }
                            }).catch(err => {
                                this.$alertError('请求超时，请刷新重试');
                                console.log(err);
                                this.isPending = false;
                            })
                        }
                    }
                });
            }
        }
    }
</script>

<style scoped lang="scss">
    .login{
        width:100%;
        height:100%;
        .login-box{
            position:absolute;
            top:50%;
            left:50%;
            transform:translate(-50%, -50%);
            width:360px;
            background:#4BC0C0;
            box-shadow: 0 0 25px #cac6c6;
            border-radius:4px;
            .login-name{
                width:100%;
                text-align: center;
                font-size:20px;
                font-weight:bold;
                padding:20px 0;
                color:#fff;
            }
            .login-form{
                box-sizing:border-box;
                background:#fff;
                padding:35px 40px 20px 25px;
                .el-form-item__label{
                    font-size:16px;
                    font-weight:bold;
                }
            }
        }
    }
</style>