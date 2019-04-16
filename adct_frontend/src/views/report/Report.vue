<template>
    <div class="report">
        <el-scrollbar style="height:100%">
            <el-form :inline="true">
                <el-form-item>
                    <el-date-picker
                            :clearable="isClear"
                            v-model="time"
                            type="datetimerange"
                            align="right"
                            value-format="yyyy-MM-dd HH:mm:ss"
                            range-separator="-"
                            :default-time="['00:00:00','23:59:59']"
                            start-placeholder="开始时间"
                            end-placeholder="结束时间">
                    </el-date-picker>
                </el-form-item>
                <el-form-item>
                    <el-button type="primary" icon="el-icon-search" @click="showCapacity">查询</el-button>
                </el-form-item>
            </el-form>
            <el-table
                    v-loading="loading"
                    :data="tableData"
                    style="width:100%">
                <el-table-column
                        prop="name"
                        label="实例名">
                </el-table-column>
                <el-table-column
                        prop="succeed"
                        sortable
                        label="成功数">
                </el-table-column>
                <el-table-column
                        prop="failed"
                        sortable
                        label="失败数">
                </el-table-column>
                <el-table-column
                        prop="total"
                        sortable
                        label="总数">
                </el-table-column>
            </el-table>
        </el-scrollbar>
        <div class="report-pagination">
            <el-button-group>
                <el-button type="primary" icon="el-icon-arrow-left" size="small" @click="reviewPage"></el-button>
                <el-button type="primary" icon="el-icon-arrow-right" size="small" @click="continuePage"></el-button>
            </el-button-group>
        </div>
    </div>
</template>

<script>
    import {showCapacityUrl} from "../../plugins/globalUrl";
    import {axiosPost} from "../../utils/fetchData";
    import {errHandler} from "../../utils/errorHandler";
    import {setInitialTime} from "../../utils/time";

    export default {
        name: "Report",
        data(){
            return{
                isPending:false,
                time:[],
                startTime:'',
                endTime:'',
                continueById:'',
                reviewById:'',
                tableData:[],
                loading:false,
                isClear:false
            }
        },
        created(){
            this.initTime();
        },
        mounted(){
            this.showCapacity();
        },
        watch:{
            time: {
                handler(value) {
                    if(value !== null){
                        this.startTime = value[0];
                        this.endTime = value[1];
                    }
                },
                deep: true
            }
        },
        methods:{
            //初始化时间
            initTime:function(){
                let time = setInitialTime();
                this.startTime = time[0] +  " 00:00:00";
                this.endTime = time[1] +  " 23:59:59";
                this.time = [this.startTime,this.endTime];
            },
            //显示产能
            showCapacity:function(){
                if(this.time === null){
                    this.$alertWarning("开始日期和结束日期不能为空");
                    return;
                }
                if(!this.isPending){
                    this.isPending = true;
                    this.loading = true;
                    let options = {
                        url:showCapacityUrl,
                        data:{
                            startTime:this.startTime,
                            endTime:this.endTime,
                            continueById:this.continueById,
                            reviewById:this.reviewById
                        }
                    };
                    axiosPost(options).then(response => {
                        this.isPending = false;
                        this.loading = false;
                        if(response.data.code === 200){
                            if(response.data.data.length === 0){
                                if(this.reviewById !== '' && this.continueById === ''){
                                    this.$alertInfo('当前是第一页');
                                }
                                if(this.reviewById === '' && this.continueById !== ''){
                                    this.$alertInfo('当前是最后一页');
                                }
                                if(this.reviewById === '' && this.continueById === ''){
                                    this.tableData = response.data.data;
                                }
                            }else{
                                this.tableData = response.data.data;
                            }
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
            },
            //上一页
            reviewPage:function(){
                if(this.tableData.length === 0){
                    this.$alertInfo('当前页面无数据');
                    return;
                }
                this.reviewById = this.tableData[0].id;
                this.continueById = '';
                this.showCapacity();
            },
            //下一页
            continuePage:function(){
                if(this.tableData.length === 0){
                    this.$alertInfo('当前页面无数据');
                    return;
                }
                this.reviewById = '';
                this.continueById = this.tableData[this.tableData.length - 1].id;
                this.showCapacity();
            }
        }
    }
</script>

<style scoped lang="scss">
    .report{
        position: relative;
        box-sizing:border-box;
        padding:40px;
        width:100%;
        height:100%;
        overflow-x:hidden;
        background:#ffffff;
        border-radius:6px;
        box-shadow: 0 2px 12px 0 rgba(0, 0, 0, 0.1);
        .report-pagination{
            position:absolute;
            bottom:10px;
            right:10px;
        }
    }
</style>