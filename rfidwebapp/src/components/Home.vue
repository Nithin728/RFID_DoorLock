<!-- Home.vue functionality -->

<template>
  <div class="home">
    Welcome to Team 32's RFID Web App!
    <router-link to="keycodes" class="links">Edit Key Codes</router-link>
    <router-link to="log" class="links">View Log</router-link>
    <router-link to="signIn" class="links" v-if="!isLoggedIn">Sign In</router-link>
    <button @click="handleSignOut" class="signOut" style="font-size: 1rem" v-if="isLoggedIn">Sign Out</button>
    <button id="mode"><i class="fas fa-moon"></i></button>
    <!-- <router-view /> -->
    <figure class="image is-16by9">
      <img src="https://img.freepik.com/premium-vector/wireless-key-card-human-hand-with-guest-room-door-handle-sensor-concept-access-identification-access-control-machine-proximity-card-reader-vector-illustration-flat-style_169241-4074.jpg">
    </figure>
  </div>
</template>

<!-- Dark Mode feature and Authentication in javascript-->
<script setup>
import { onMounted, ref } from 'vue';
import { getAuth, onAuthStateChanged, signOut } from "firebase/auth";
import { router } from '../router';

const isLoggedIn = ref(false);

let auth;
onMounted(() => {
  auth = getAuth();
  onAuthStateChanged(auth, (user) => {
    if (user) {
      isLoggedIn.value = true;
    } else {
      isLoggedIn.value = false;
    }
  });
});

const handleSignOut = () => {
  signOut(auth).then(() => {
    console.log("Successfully Signed Out");
    router.push('/');
  });
};

onMounted(() => {
  const body = document.querySelector('body');
  const toggle = document.getElementById('mode');

  if (!body || !mode) {
    console.error('Body or mode element not found');
    return;
  }

  toggle.addEventListener('click', () => {
    if (body.classList.contains('dark')) {
      body.className = '';
    } else {
      body.className = 'dark';
    }
  });
});
</script>

<!-- Using bulma to make the frontend look nicer -->
<style>
  @import 'bulma/css/bulma.min.css';
  
  .home { 
    /* margin: 0 auto; */
    font-size: 1.5rem; /* 1 rem = 16px */
    padding: 1rem; 
    text-align: center;
    overflow: auto;
  }
  .links {
    display: block;
    /* margin: 0 auto; */
    /* width: 50%; */
    font-size: 1.5rem; 
    padding: 1rem;
    text-align: center;
    border-radius: 20px;
    overflow: auto;
  }
  .image {
    display: block;
    margin: 0 auto;
    margin-top: 1rem;
    width: 100%;
    padding: 1rem;
    text-align: center;
    border-radius: 20px;
    overflow: auto;
  }

  .signOut {
    margin: 0 auto;
    margin-bottom: 1rem;
    padding: 1rem;
    text-align: center;
    border-radius: 20px;
    overflow: auto;
  }

  @media screen and (max-width: 768px) 
  {
    .home 
    {
      font-size: 24px; /* Reduced size */
      padding: 10px; /* Reduced padding */
      overflow: auto;
    }
    .links 
    {
      display: block;
      font-size: 18px; /* Reduced size */
      padding: 15px; /* Adjusted padding */
      border-radius: 10px;
      overflow: auto;
    }
  }
</style>

<!-- Dark Mode styling in css -->
<style lang="scss">
  @import 'node_modules/bulma/bulma.sass';

  /* //styles.scss */
  /* // Set your brand colors */
  $custom-light: #ffffff;
  $custom-dark: #1b191e;

/* // Update Bulma's global variables */
  $dark: $custom-dark;
  $light: $custom-light;
  body {
    background-color: $light;
    min-height: 100vh;
    transition: all 0.4s ease;
  }

  /* // Some styling  */

  body .subtitle,
  body .title {
    color: $dark;
  }

  body.dark {
    background-color: $dark;
    color: $light;
  }

  body.dark .subtitle,
  body.dark .title {
    color: $light;
  }

  .section {
    display: flex;
    align-items: center;
    justify-content: center;
    min-height: 100vh;
    text-align: center;
  }

  .button{
    cursor: pointer;
    border: none;
    color: $light;
    padding: 0.8rem;
    margin-bottom: 1rem;
    margin-right: 1rem;  
    background: $dark;
    border-radius: 2rem;
    text-align: center;
    // font-size: 1rem
  }

  body.dark button {
    background: $light;
    color: $dark;
  }
</style>