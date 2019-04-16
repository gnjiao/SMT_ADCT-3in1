<template>
    <div class="example">
        <el-scrollbar style="height:100%;">
            <div class="example-box" v-loading="loading">
                <div class="example-box-header">
                    <div class="example-form">
                        <el-form :inline="true" class="demo-form-inline" @submit.native.prevent>
                            <el-form-item>
                                <el-input  placeholder="实例名" v-model.trim="inputValue" spellcheck="false" autocomplete="false"></el-input>
                            </el-form-item>
                            <el-form-item>
                                <el-button type="primary" icon="el-icon-search" @click="search">查询</el-button>
                            </el-form-item>
                        </el-form>
                    </div>
                </div>
                <div class="example-box-main">
                    <el-row :gutter="20">
                        <el-col :xs="24" :sm="12" :md="8" :lg="6" :xl="4"
                                v-for="item in instances"
                                :key="item.id">
                            <div class="example-details" @click="operateInstance(item)">
                                <span class="example-icon-close"
                                      v-if="item.online === false"
                                      @click="confirmDelete(item)">
                                    <i class="el-icon-close"></i>
                                </span>
                                <span class="example-icon-example" >
                                    <i class="el-icon-coke-example"  :style="instanceColor(item.online)"></i>
                                </span>
                                <span class="example-name">{{item.name}}</span>
                            </div>
                        </el-col>
                    </el-row>
                </div>
            </div>
        </el-scrollbar>
        <div class="example-pagination">
            <el-button-group>
                <el-button type="primary" icon="el-icon-arrow-left" size="small" @click="reviewPage"></el-button>
                <el-button type="primary" icon="el-icon-arrow-right" size="small" @click="continuePage"></el-button>
            </el-button-group>
        </div>
        <instance-dialog></instance-dialog>
    </div>
</template>

<script>
    import {mapActions,mapGetters} from 'vuex'
    import {deleteInstanceUrl, showInstancesUrl} from "../../plugins/globalUrl";
    import {axiosPost} from "../../utils/fetchData";
    import {errHandler} from "../../utils/errorHandler";
    import Bus from './../../utils/bus'
    import InstanceDialog from "./component/InstanceDialog";
    export default {
        name: "Example",
        components: {InstanceDialog},
        data(){
            return{
                validateEvent:false,
                isPending:false,
                continueById:'',
                reviewById:'',
                inputValue:'',
                instanceNameKey:'',
                instances:[],
                loading:false,
                //定时器
                myTimeOut:'',
                //是否启用定时器
                isTimeOut:false
            }
        },
        beforeDestroy(){
            this.clearMyTimeOut();
        },
        mounted(){
            this.setMyTimeOut();
        },
        computed:{
            ...mapGetters(['page'])
        },
        methods:{
            ...mapActions(['setPage']),
            //显示实例
            showInstances:function(){
                if(!this.isPending){
                    this.isPending = true;
                    this.loading = true;
                    let options = {
                        url:showInstancesUrl,
                        data:{
                            continueById:this.continueById,
                            reviewById:this.reviewById,
                            instanceNameKey:this.instanceNameKey
                        }
                    };
                    axiosPost(options).then(response => {
                        this.isPending = false;
                        this.loading = false;
                        if(response.data.code === 200){
                            if(response.data.data.length === 0){
                                if(this.reviewById !== '' && this.continueById === ''){
                                    this.$alertInfo('当前是第一页');
                                    this.reviewById = this.page.reviewById;
                                    this.continueById = this.page.continueById;
                                }
                                if(this.reviewById === '' && this.continueById !== ''){
                                    this.$alertInfo('当前是最后一页');
                                    this.reviewById = this.page.reviewById;
                                    this.continueById = this.page.continueById;
                                }
                                if(this.reviewById === '' && this.continueById === ''){
                                    this.instances = response.data.data;
                                }
                            }else{
                                this.instances = response.data.data;
                            }
                        }else{
                            errHandler(response.data);
                        }
                    }).catch(err => {
                        this.isPending = false;
                        this.loading = false;
                        console.log(err);
                        this.$alertError('请求超时，请刷新重试');
                    })
                }
            },
            //查询
            search:function(){
                this.reviewById = '';
                this.continueById = '';
                this.instanceNameKey = this.inputValue;
                this.showInstances();
            },
            //设置实例颜色
            instanceColor:function(val){
                let color = val===true?'#4BC0C0':"#F56C6C";
                return {
                    color:color,
                    fontSize:'60px',
                }
            },
            //上一页
            reviewPage:function () {
                if(this.instances.length === 0){
                    this.$alertInfo('当前页面无数据');
                    return;
                }
                this.setPage({
                    reviewById:this.reviewById,
                    continueById:this.continueById
                });
                this.reviewById = this.instances[0].id;
                this.continueById = '';
                this.showInstances();
            },
            //下一页
            continuePage:function(){
                if(this.instances.length === 0){
                    this.$alertInfo('当前页面无数据');
                    return;
                }
                this.setPage({
                    reviewById:this.reviewById,
                    continueById:this.continueById
                });
                this.reviewById = '';
                this.continueById = this.instances[this.instances.length - 1].id;
                this.showInstances();
            },
            //确认删除
            confirmDelete:function(item){
                this.$confirm('是否删除实例名为“'+item.name+'”的离线实例?\n删除该实例时会删除该实例的所有日志，请慎重选择。', {
                    confirmButtonText: '确定',
                    cancelButtonText: '取消',
                    type: 'warning',
                    center: false
                }).then((action) => {
                    if (action === "confirm") {
                        this.deleteInstance(item.id)
                    }
                }).catch(() => {
                    this.$alertInfo("已取消删除");
                });
            },
            //删除离线实例
            deleteInstance:function (id) {
                if(!this.isPending){
                    this.isPending = true;
                    let options = {
                        url:deleteInstanceUrl,
                        data:{
                            id:id
                        }
                    };
                    axiosPost(options).then(response => {
                        this.isPending = false;
                        if(response.data.code === 200){
                            this.$alertWarning('删除成功');
                            this.showInstances();
                        }else{
                            errHandler(response.data);
                        }
                    }).catch(err => {
                        this.isPending = false;
                        console.log(err);
                        this.$alertError('连接超时，请刷新重试')
                    })
                }
            },
            //实例操作
            operateInstance:function(item){
                if(item.online === false){
                    return;
                }
                this.clearMyTimeOut();
                Bus.$emit('showInstance',item);
            },
            //设置定时器
            setMyTimeOut:function(){
                if(this.isTimeOut === true){
                    this.clearMyTimeOut();
                }
                this.showInstances();
                this.isTimeOut = true;
                let that = this;
                this.myTimeOut = setInterval(function () {
                    that.showInstances();
                },10000);
            },
            //清除定时器
            clearMyTimeOut:function(){
                this.isTimeOut = false;
                clearTimeout(this.myTimeOut);
                this.myTimeOut = null;
            },
        }
    }
</script>

<style scoped lang="scss">
    .example{
        position:relative;
        box-sizing:border-box;
        padding-top:10px;
        width:100%;
        height:100%;
        overflow-x:hidden;
        .example-box{
            .example-box-header{
                .example-form{
                    float:right;
                }
            }
            .example-box-main{
                width:100%;
                padding-top:60px;
                min-height:400px;
                .el-row{
                    width:100%;
                    .example-details{
                        position:relative;
                        display:flex;
                        flex-direction:column;
                        align-items:center;
                        box-sizing:border-box;
                        padding:20px 0 10px 0;
                        border-radius:6px;
                        cursor:pointer;
                        transition: 1s;
                        text-align:center;
                        margin-bottom:20px;
                        .example-icon-close{
                            color:#f4f3ef;
                            position:absolute;
                            top:8px;
                            right:8px;
                            i{
                                font-size:20px;
                            }
                        }
                    }
                    .example-details:hover{
                        background:#fff;
                        box-shadow: 0 2px 8px 0 rgba(0, 0, 0, 0.1);
                        .example-icon-close{
                            color:#767676
                        }
                    }
                }
            }
        }
        .example-pagination{
            background:#fff;
            position:absolute;
            right:0;
            bottom:0;
        }
    }
</style>