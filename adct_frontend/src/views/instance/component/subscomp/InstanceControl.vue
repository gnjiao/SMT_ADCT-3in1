<template>
    <div class="control">
        <el-scrollbar style="height:100%">
            <div class="control-form">
                <div class="control-state">
                    <span class="instance-icon"><i class="el-icon-coke-example" style="font-size:80px;color:#ccc"></i></span>
                    <span class="instance-state"><span>状态：</span>{{stateString}}</span>
                </div>
                <el-form :model="controlForm" ref="controlForm" label-width="50px">
                    <el-form-item label="订单">
                        <el-select
                                filterable
                                remote
                                placeholder="请输入关键词"
                                :remote-method="remoteMethod"
                                :loading="loading"
                                v-model="controlForm.order"
                                value="">
                            <el-option
                                v-for="(item,index) in orders"
                                :key="index"
                                :label="item"
                                :value="item"
                            ></el-option>
                        </el-select>
                    </el-form-item>
                    <el-form-item label="版本">
                        <el-input v-model="controlForm.version" type="text"></el-input>
                    </el-form-item>
                    <el-form-item>
                        <el-button type="info" size="medium"  :disabled="state !== 'not-start'" @click="cancelInstance">取消</el-button>
                        <el-button type="primary" size="medium"  :disabled="state !== 'not-bind'" @click="bindInstance">绑定</el-button>
                        <el-button type="primary" size="medium"  :disabled="state !== 'not-start'" @click="startInstance">开始</el-button>
                        <el-button type="danger" size="medium"  :disabled="state !== 'running'" @click="stopInstance">停止</el-button>
                    </el-form-item>
                </el-form>
            </div>
        </el-scrollbar>
    </div>
</template>

<script>
    import Bus from './../../../../utils/bus'
    import {bindUrl, cancelUrl, getStateUrl, searchOrdersUrl, startUrl, stopUrl} from "../../../../plugins/globalUrl";
    import {axiosPost} from "../../../../utils/fetchData";
    import {errHandler} from "../../../../utils/errorHandler";

    export default {
        name: "InstanceControl",
        props:{
            instanceId:Number,
            dialogVisible:Boolean
        },
        data(){
            return{
                controlForm:{
                    order:'',
                    version:''
                },
                //订单号
                orders:[],
                //当前实例状态
                state:'',
                loading:false,
                isPending:false,
                //定时器
                myTimeOut:'',
                //是否启用定时器
                isTimeOut:false
            }
        },
        beforeDestroy(){
            this.clearMyTimeOut();
            Bus.$off('setControlTimeOut');
        },
        computed:{
            stateString:function(){
                let result = '';
                switch(this.state){
                    case 'not-start':
                        result = '未开始';
                        break;
                    case 'not-bind':
                        result = '未绑定';
                        break;
                    case 'running':
                        result = '运行中';
                        break;
                    default:
                        result = '未知';
                        break;
                }
                return result;
            }
        },
        watch:{
            dialogVisible:function(val){
                if(val === true){
                    this.setMyTimeOut();
                }else{
                    this.clearMyTimeOut();
                    this.initData();
                }
            }
        },
        mounted(){
            this.setMyTimeOut();
            Bus.$on('setControlTimeOut',(index) => {
                if(index !== '0'){
                    this.clearMyTimeOut();
                }else{
                    this.setMyTimeOut();
                }
            })
        },
        methods:{
            //初始化
            initData:function(){
                this.controlForm.version = '';
                this.controlForm.order = '';
            },
            //模糊查询订单号
            remoteMethod:function(val){
                if(val !== ''){
                    if(!this.isPending){
                        this.isPending = true;
                        this.loading = true;
                        let options = {
                            url:searchOrdersUrl,
                            data:{
                                instanceId: this.instanceId,
                                orderKey:val
                            }
                        };
                        axiosPost(options).then(response => {
                            this.isPending = false;
                            this.loading = false;
                            if(response.data.code === 200){
                                this.orders = response.data.data;
                            }else{
                                errHandler(response.data);
                            }
                        }).catch(err => {
                            console.log(err);
                            this.isPending = false;
                            this.loading = false;
                            this.$alertError('连接超时，请刷新重试');
                        })
                    }
                }else{
                    this.orders = []
                }
            },
            //实时获取实例工作状态
            getState:function(){
                if(!this.isPending){
                    this.isPending = true;
                    let options = {
                        url:getStateUrl,
                        data:{
                            instanceId:this.instanceId
                        }
                    };
                    axiosPost(options).then(response => {
                        this.isPending = false;
                        if(response.data.code === 200){
                            this.state = response.data.data;
                        }else{
                            this.state = '';
                            errHandler(response.data);
                        }
                    }).catch(err => {
                        console.log(err);
                        this.state = '';
                        this.isPending = false;
                        this.$alertError('请求超时，请刷新重试');
                    })
                }
            },
            //设置定时器
            setMyTimeOut:function(){
                if(this.isTimeOut === true){
                    this.clearMyTimeOut();
                }
                this.getState();
                this.isTimeOut = true;
                let that = this;
                this.myTimeOut = setInterval(function () {
                    that.getState();
                },10000);
            },
            //清除定时器
            clearMyTimeOut:function(){
                this.isTimeOut = false;
                clearTimeout(this.myTimeOut);
                this.myTimeOut = null;
            },
            //停止工作
            stopInstance:function(){
                if(this.isTimeOut === true){
                    this.clearMyTimeOut();
                }
                if(!this.isPending){
                    this.isPending = true;
                    let options = {
                        url:stopUrl,
                        data:{
                            instanceId:this.instanceId
                        }
                    };
                    axiosPost(options).then(response => {
                        if(response.data.code === 200){
                            this.$alertSuccess('停止成功');
                        }else{
                            errHandler(response.data);
                        }
                        this.isPending = false;
                        this.setMyTimeOut();
                    }).catch(err => {
                        console.log(err);
                        this.isPending = false;
                        this.setMyTimeOut();
                        this.$alertError('连接超时，请刷新重试');
                    })
                }
            },
            //开始工作
            startInstance:function(){
                if(this.isTimeOut === true){
                    this.clearMyTimeOut();
                }
                if(!this.isPending){
                    this.isPending = true;
                    let options = {
                        url:startUrl,
                        data:{
                            instanceId:this.instanceId
                        }
                    };
                    axiosPost(options).then(response => {
                        if(response.data.code === 200){
                            this.$alertSuccess('启用成功');
                        }else{
                            errHandler(response.data);
                        }
                        this.isPending = false;
                        this.setMyTimeOut();
                    }).catch(err => {
                        console.log(err);
                        this.isPending = false;
                        this.setMyTimeOut();
                        this.$alertError('连接超时，请刷新重试');
                    })
                }
            },
            //取消绑定
            cancelInstance:function () {
                if(this.isTimeOut === true){
                    this.clearMyTimeOut();
                }
                if(!this.isPending){
                    this.isPending = true;
                    let options = {
                        url:cancelUrl,
                        data:{
                            instanceId:this.instanceId
                        }
                    };
                    axiosPost(options).then(response => {
                        if(response.data.code === 200){
                            this.$alertSuccess('取消成功');
                        }else{
                            errHandler(response.data);
                        }
                        this.isPending = false;
                        this.setMyTimeOut();
                    }).catch(err => {
                        console.log(err);
                        this.isPending = false;
                        this.setMyTimeOut();
                        this.$alertError('连接超时，请刷新重试');
                    })
                }
            },
            //绑定订单
            bindInstance:function(){
                if(this.controlForm.order === '' || this.controlForm.version === ''){
                    this.$alertWarning('订单号和版本号不能为空');
                    return;
                }
                if(this.isTimeOut === true){
                    this.clearMyTimeOut();
                }
                if(!this.isPending){
                    this.isPending = true;
                    let options = {
                        url:bindUrl,
                        data:{
                            instanceId:this.instanceId,
                            order:this.controlForm.order,
                            version:this.controlForm.version
                        }
                    };
                    axiosPost(options).then(response => {
                        if(response.data.code === 200){
                            this.$alertSuccess('绑定成功');
                        }else{
                            errHandler(response.data);
                        }
                        this.isPending = false;
                        this.setMyTimeOut();
                    }).catch(err => {
                        console.log(err);
                        this.isPending = false;
                        this.setMyTimeOut();
                        this.$alertError('连接超时，请刷新重试');
                    })
                }
            }
        }
    }
</script>

<style scoped lang="scss">
    .control{
        position:relative;
        box-sizing:border-box;
        width:100%;
        height:100%;
        padding-bottom: 10px;
        .control-form{
            box-sizing:border-box;
            position:absolute;
            top:50%;
            left:50%;
            transform: translate(-50%,-50%);
            padding:20px 10px 0 0;
            .control-state{
                display: flex;
                flex-direction: column;
                align-items:center;
                margin-bottom:20px;
                .instance-state{
                    font-size:18px;
                    color:#797979;
                    font-weight:bold;
                    span{
                        color:#ccc;
                    }
                }
            }
        }
    }
</style>