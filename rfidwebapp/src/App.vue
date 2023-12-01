<!-- App.vue functionality -->

<template>
  <div id="app">
    <router-view/>
  </div>
</template>

<!-- Using bulma to make the frontend look nicer -->
<style>
  @import 'bulma/css/bulma.min.css';

  @media (min-width: 1024px) {
    #app { 
      display: block !important;
    }
  }
</style>