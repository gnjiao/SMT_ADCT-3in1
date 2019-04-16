<template>
    <div class="log">
        <el-scrollbar style="height:100%">
            <el-table
                    ref="filterTable"
                    v-loading="loading"
                    :data="tableData"
                    style="width:100%">
                <el-table-column
                        prop="time"
                        label="日期"
                        width="250"
                        sortable>
                </el-table-column>
                <el-table-column
                        prop="type"
                        width="180"
                        label="状态"
                        :filters="[
                            {text: 'succeed', value: 'succeed'},
                            {text: 'error', value: 'error'},
                            {text: 'failed', value: 'failed'},
                            {text: 'info', value: 'info'},
                            {text: 'warn', value: 'warn'}
                          ]"
                        :filter-method="filterHandler">
                    <template slot-scope="scope">
                        <span
                                style="font-weight:bold"
                                :class="getColor(scope.row.type)"
                        >
                            {{scope.row.type}}
                        </span>
                    </template>
                </el-table-column>
                <el-table-column
                        prop="message"
                        label="信息">
                    <template slot-scope="scope">
                        <span
                                style="font-weight:bold"
                                :class="getColor(scope.row.type)"
                        >
                            {{scope.row.message}}
                        </span>
                    </template>
                </el-table-column>
            </el-table>
        </el-scrollbar>
        <div class="log-pagination">
            <el-button-group>
                <el-button type="primary" icon="el-icon-arrow-left" size="mini" @click="reviewPage"></el-button>
                <el-button type="primary" icon="el-icon-arrow-right" size="mini" @click="continuePage"></el-button>
            </el-button-group>
        </div>
    </div>
</template>

<script>
    import Bus from './../../../../utils/bus'
    import {showLogsUrl} from "../../../../plugins/globalUrl";
    import {axiosPost} from "../../../../utils/fetchData";
    import {errHandler} from "../../../../utils/errorHandler";

    export default {
        name: "InstanceLog",
        props: {
            instanceId: Number,
            dialogVisible:Boolean
        },
        data() {
            return {
                isPending: false,
                continueById: '',
                reviewById: '',
                tableData: [],
                loading: false
            }
        },
        beforeDestroy() {
            Bus.$off('showLogs');
        },
        watch:{
            dialogVisible:function(val){
                if(val === false){
                    this.$refs.filterTable.clearFilter();
                }
            }
        },
        mounted() {
            Bus.$on('showLogs', (index) => {
                if (index === '1') {
                    this.showLogs();
                }
            })
        },
        methods: {
            //显示日志
            showLogs: function () {
                if (!this.isPending) {
                    this.isPending = true;
                    this.loading = true;
                    let options = {
                        url: showLogsUrl,
                        data: {
                            continueById: this.continueById,
                            reviewById: this.reviewById,
                            instanceId: this.instanceId
                        }
                    };
                    axiosPost(options).then(response => {
                        this.isPending = false;
                        this.loading = false;
                        if (response.data.code === 200) {
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
                        } else {
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
            //根据状态获取颜色
            getColor: function (type) {
                let colorClass = '';
                switch (type) {
                    case 'succeed':
                        colorClass = 'succeed-row';
                        break;
                    case 'failed':
                        colorClass = 'failed-row';
                        break;
                    case 'error':
                        colorClass = 'error-row';
                        break;
                    case 'warn':
                        colorClass = 'warn-row';
                        break;
                    case 'info':
                        colorClass = 'info-row';
                        break;
                    default:
                        break;
                }
                return colorClass;
            },
            //过滤
            filterHandler: function (value, row, column) {
                const property = column['property'];
                return row[property] === value;
            },
            //上一页
            reviewPage:function(){
                if(this.tableData.length === 0){
                    this.$alertInfo('当前页面无数据');
                    return;
                }
                this.reviewById = this.tableData[0].id;
                this.continueById = '';
                this.showLogs();
            },
            //下一页
            continuePage:function(){
                if(this.tableData.length === 0){
                    this.$alertInfo('当前页面无数据');
                    return;
                }
                this.reviewById = '';
                this.continueById = this.tableData[this.tableData.length - 1].id;
                this.showLogs();
            }
        }
    }
</script>

<style scoped lang="scss">
    .log {
        position:relative;
        box-sizing: border-box;
        width: 100%;
        height: 100%;
        padding-bottom: 10px;
        .log-pagination{
            position:absolute;
            bottom:10px;
            right:0;
        }
    }
</style>