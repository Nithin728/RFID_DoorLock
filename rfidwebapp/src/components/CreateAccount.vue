<template>
  <div class="createAccount">
    <!-- Title of the webpage -->
    <div class="title has-text-centered" style="font-size: 2rem; margin-bottom: 2rem;">
      Create an Account
    </div>
  </div>
    <div class="form-container">
      <div class="field">
        <p class="control has-icons-left has-icons-right">
          <input class="input" type="email" placeholder="Email" v-model="email" style="font-size: 1rem; height: 2rem;">
          <span class="icon is-small is-left">
            <i class="fas fa-envelope"></i>
          </span>
          <span class="icon is-small is-right">
            <i class="fas fa-check"></i>
          </span>
        </p>
      </div>
      <div class="field">
        <p class="control has-icons-left">
          <input class="input" type="password" placeholder="Password" v-model="password" style="font-size: 1rem; height: 2rem;">
          <span class="icon is-small is-left">
            <i class="fas fa-lock"></i>
          </span>
        </p>
      </div>
      <p><button @click="register" style="font-size: 1rem;">Submit</button></p>
    </div>
</template>

<script setup>
import { ref } from 'vue'
import { getAuth, createUserWithEmailAndPassword } from "firebase/auth";
import { doc, setDoc } from "firebase/firestore";
import { db } from '@/firebase'; // adjust the path to point to your firebase.js file
import { useRouter } from 'vue-router';
const email = ref('')
const password = ref('')
const router = useRouter();

// const register = () => {
//   createUserWithEmailAndPassword(getAuth(), email.value, password.value)
//     .then((data) => {
//       console.log("Successfully Registered");
//       router.push('/');
//     })
//     .catch((error) => {
//       console.log(error.code);
//       alert(error.message);
//     });
// };

const register = () => {
  const auth = getAuth();

  createUserWithEmailAndPassword(auth, email.value, password.value)
    .then((userCredential) => {
      // Successfully created a new user account
      const user = userCredential.user;

      // Create a new document in the 'users' collection with the same ID as the User UID
      const userDocRef = doc(db, 'users', user.uid);
      return setDoc(userDocRef, {
        email: user.email,
        // ... any other initial data you want to set for the user ...
      });
    })
    .then(() => {
      console.log("Successfully Registered and Document written!");
      router.push('/');
    })
    .catch((error) => {
      console.log(error.code);
      alert(error.message);
    });
};


</script>

<style>

  @import 'bulma/css/bulma.min.css';

  /* Center the form-container */
  .form-container {
    text-align: center;
    margin: auto;
  }

  /* Center the inputs and set their styles */
  .input {
    display: inline-block;
    font-size: 1.5rem;
    padding: 1rem;
    text-align: center;
    margin: auto;
    display: block;
    margin-bottom: 20px;  /* Add margin-bottom to create space below each input */
  }

  /* Button styles */
  button {
    cursor: pointer;
    border: none;
    padding: .8rem;
    margin-bottom: 20px;  /* Add margin-bottom to create space below each button */
    border-radius: 2rem;
    text-align: center;
    margin: auto;
    display: block;
  }

  /* Add some spacing between text paragraphs */
  p {
    margin-bottom: 20px;  /* Add margin-bottom to create space below each paragraph */
  }

</style>