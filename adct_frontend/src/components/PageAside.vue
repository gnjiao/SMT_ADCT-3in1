<template>
    <div class="aside">
        <el-menu :default-active="activeRoute"
                 class="el-menu-vertical-demo"
                 text-color="#666"
                 router
                 :collapse="isCollapse">
            <el-scrollbar style="height:100%">
                <el-menu-item index="instance">
                    <i class="el-icon-coke-example"></i>
                    <span slot="title">实例操作</span>
                </el-menu-item>
                <el-menu-item index="report">
                    <i class="el-icon-coke-report"></i>
                    <span slot="title">统计报表</span>
                </el-menu-item>
            </el-scrollbar>
        </el-menu>
    </div>
</template>

<script>
    import Bus from './../utils/bus.js'

    export default {
        name: "PageAside",
        data() {
            return {
                isCollapse:true,
                activeRoute: 'instance'
            }
        },
        beforeDestroy() {
            Bus.$off('collapse');
        },
        created() {
            this.activeRoute = this.$route.name;
        },
        mounted() {
            Bus.$on('collapse', () => {
                this.isCollapse = !this.isCollapse;
            })
        }
    }
</script>

<style scoped lang="scss">
    .aside {
        display: flex;
        height: 100%;
        .el-menu-vertical-demo {
            box-sizing: border-box;
            height: 100%;
            padding-top: 20px;
            box-shadow: 0 2px 12px 0 rgba(0, 0, 0, 0.1);
        }
        .el-menu-vertical-demo:not(.el-menu--collapse) {
            text-align: left;
            width:180px;
        }
        .el-menu-item {
            font-size: 16px;
            i {
                font-size:20px;
                margin-right: 10px;
            }
        }
    }
</style>