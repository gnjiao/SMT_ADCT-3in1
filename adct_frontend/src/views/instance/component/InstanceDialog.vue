<template>
    <el-dialog
            :title="item.name"
            :close-on-click-modal="isCloseOnModal"
            :close-on-press-escape="isCloseOnModal"
            :visible.sync="instanceDialogVisible">
        <el-tabs
                v-model="tabIndex"
                @tab-click="tabClick"
                tab-position="top">
            <el-tab-pane >
                <span slot="label">
                    <i class="el-icon-coke-example" style="margin-right:5px;font-size:18px;"></i>
                    远程控制
                </span>
                <instance-control
                        :instanceId="item.id"
                        :dialogVisible="instanceDialogVisible"
                ></instance-control>
            </el-tab-pane>
            <el-tab-pane >
                <span slot="label">
                    <i class="el-icon-coke-report" style="margin-right:5px;font-size:18px;"></i>
                    查看日志
                </span>
                <instance-log
                        :instanceId="item.id"
                        :dialogVisible="instanceDialogVisible"
                ></instance-log>
            </el-tab-pane>
        </el-tabs>
    </el-dialog>
</template>

<script>
    import Bus from './../../../utils/bus'
    import InstanceControl from "./subscomp/InstanceControl";
    import InstanceLog from "./subscomp/InstanceLog";
    export default {
        name: "InstanceDialog",
        components: {
            InstanceLog,
            InstanceControl
        },
        data(){
            return{
                instanceDialogVisible:false,
                isCloseOnModal:false,
                item:{},
                tabIndex:'0'
            }
        },
        beforeDestroy(){
            Bus.$off('showInstance');
        },
        watch:{
            instanceDialogVisible:function(val){
                if(val === false) {
                    this.tabIndex = '0';
                    this.$parent.setMyTimeOut();
                }
            }
        },
        mounted(){
            Bus.$on('showInstance',item => {
                this.item = item;
                this.instanceDialogVisible = true;
            })
        },
        methods:{
            tabClick:function(val){
                Bus.$emit('setControlTimeOut',val.index);
                Bus.$emit('showLogs',val.index);
            }
        }
    }
</script>

<style scoped lang="scss">

</style>