import { createApp } from 'vue' 
import App from './App.vue'
import { router } from './router/index.js'
import './assets/main.css'

//initialize the app
createApp(App).use(router).mount('#app')
