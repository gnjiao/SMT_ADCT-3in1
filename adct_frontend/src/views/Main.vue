<template>
    <el-container class="page">
        <el-header class="page-header">
            <page-header></page-header>
        </el-header>
        <el-container>
            <page-aside class="page-aside"></page-aside>
            <el-main class="page-main">
                <router-view/>
            </el-main>
        </el-container>
    </el-container>
</template>

<script>
    import PageHeader from "../components/PageHeader";
    import PageAside from "../components/PageAside";

    export default {
        name: "Main",
        components: {PageAside, PageHeader}
    }
</script>

<style scoped lang="scss">
    .page {
        width: 100%;
        height: 100%;
        background: rgba(0, 0, 0, 0);
        .page-header {
            background: #4BC0C0;
        }
    }
</style>