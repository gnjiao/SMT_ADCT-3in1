<template>
  <div id="app">
    <router-view></router-view>
  </div>
</template>

<style lang="scss">
  html, body, #app, ul, li {
    margin: 0;
    padding: 0;
  }

  html, body, #app {
    width: 100%;
    height: 100%;
  }

  ul, li {
    list-style: none;
  }

  #app {
    font-family:'微软雅黑', 'Avenir', Helvetica, Arial, sans-serif;
    -webkit-font-smoothing:antialiased;
    -moz-osx-font-smoothing:grayscale;
    color: #797979;
    background:#f4f3ef;
  }
</style>
