// import Vue from 'vue'
import { createRouter, createWebHistory } from 'vue-router'
import {getAuth, onAuthStateChanged } from "firebase/auth";
import Keycodes from '../components/Keycodes.vue'
import Log from '../components/Log.vue'
import Home from '../components/Home.vue'
import SignIn from '../components/SignIn.vue'
import CreateAccount from '../components/CreateAccount.vue'

//holds all of the routes for the app`
const routes = [
    {
        path: '/',
        name: 'home',
        component: Home
    },
    {
        path: '/keycodes',
        name: 'keycodes',
        component: Keycodes,
        meta: {
            requiresAuth: true,
        }
    },
    {
        path: '/log',
        name: 'log',
        component: Log,
        meta: {
            requiresAuth: true,
        }
    },
    {
        path: '/signIn',
        name: 'signIn',
        component: SignIn
    },
    {
        path: '/createAccount',
        name: 'createAccount',
        component: CreateAccount
    }
]

const router = createRouter({
    history: createWebHistory(),
    routes
})

const getCurrentUser = () => {
    return new Promise((resolve, reject) => {
        const removeListener = onAuthStateChanged(
            getAuth(),
            (user) => {
                removeListener();
                resolve(user);
            },
            reject
        );
    });
};

router.beforeEach(async (to, from, next) => {
    if (to.matched.some((record) => record.meta.requiresAuth)) {
        if (await getCurrentUser()) {
            next();
        } else {
            alert("you don't have access!");
            next("/");
        }
    } else {
        next();
    }
});

export { router }