<template>
  <div class="signIn">
    <div class="title has-text-centered" style="font-size: 2.0rem;">
      Sign Into Your Account
    </div>
    <!-- Form Container -->
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
      <p v-if="errMsg">{{ errMsg }}</p>
      <p><button @click="register" style="font-size: 1rem;">Submit</button></p>
      <p><button @click="signInWithGoogle" style="font-size: 1rem;">Sign In With Google</button></p>
      <router-link to="createAccount" class="links" style="font-size: 1.5rem;">Create an Account</router-link>
    </div>
  </div>
</template>


<!-- <script setup>
import { ref } from 'vue'
import { getAuth,signInWithEmailAndPassword, GoogleAuthProvider, signInWithPopup } from "firebase/auth";
import { useRouter } from 'vue-router';
import { doc, setDoc, getDoc } from "firebase/firestore";
import { db } from '@/firebase'; // Assuming you've initialized Firestore in firebase.js or similar
const email = ref('')
const password = ref('')
const errMsg = ref()
const router = useRouter();

const register = () => {
  signInWithEmailAndPassword(getAuth(), email.value, password.value)
    .then((data) => {
      console.log("Successfully Signed In");
      router.push('/');
    })
    .catch((error) => {
      console.log(error.code);
      switch (error.code) {
        case "auth/invalid-email":
          errMsg.value = "Invalid Email";
          break;
        case "auth/user-not-found":
          errMsg.value = "User Not Found";
          break;
        case "auth/wrong-password":
          errMsg.value = "Incorrect Password";
          break;
        default:
          errMsg.value = "Email or Password was Incorrect";
          break;
      }
      alert(error.message);
    });
};
// const signInWithGoogle = () => {
//   const provider = new GoogleAuthProvider();
//   signInWithPopup(getAuth(), provider)
//     .then((result) => {
//       console.log(result.user);
//       router.push('/');
//     })
//     .catch((error) => {
//       console.log(error.code);
//       alert(error.message);
//     });
// };


const signInWithGoogle = () => {
  const provider = new GoogleAuthProvider();
  const auth = getAuth();

  signInWithPopup(auth, provider)
    .then(async (result) => {
      const user = result.user;

      // Reference to the user's document in the 'users' collection
      const userDocRef = doc(db, 'users', user.uid);

      // Check if the user's document already exists
      const userDocSnapshot = await getDoc(userDocRef);
      if (!userDocSnapshot.exists()) {
        // If the document doesn't exist, create it
        await setDoc(userDocRef, {
          email: user.email,
          // ... any other initial data you want to set for the user ...
        });
      }

      console.log("User signed in:", user);
      router.push('/');
    })
    .catch((error) => {
      console.log(error.code);
      alert(error.message);
    });
};
</script> -->

<script setup>
  import { ref } from 'vue';
  import { getAuth, signInWithEmailAndPassword, GoogleAuthProvider, signInWithPopup } from "firebase/auth";
  import { getDatabase, ref as dbRef, set, get } from "firebase/database"; // Import RTDB methods
  import { useRouter } from 'vue-router';
  import { db } from '@/firebase'; // Assuming you've initialized Firebase in firebase.js or similar

  const email = ref('');
  const password = ref('');
  const errMsg = ref('');
  const router = useRouter();

  // ... existing signInWithEmailAndPassword code ...
  const register = () => {
    signInWithEmailAndPassword(getAuth(), email.value, password.value)
      .then((data) => {
        console.log("Successfully Signed In");
        router.push('/');
      })
      .catch((error) => {
        console.log(error.code);
        switch (error.code) {
          case "auth/invalid-email":
            errMsg.value = "Invalid Email";
            break;
          case "auth/user-not-found":
            errMsg.value = "User Not Found";
            break;
          case "auth/wrong-password":
            errMsg.value = "Incorrect Password";
            break;
          default:
            errMsg.value = "Email or Password was Incorrect";
            break;
        }
        alert(error.message);
      });
  };

  const signInWithGoogle = () => {
    const provider = new GoogleAuthProvider();
    const auth = getAuth();

    signInWithPopup(auth, provider)
      .then(async (result) => {
        const user = result.user;
        const database = getDatabase();

        // Reference to the user's node in RTDB
        const userRef = dbRef(database, 'users/' + user.uid);

        // Check if the user's data already exists
        const userSnapshot = await get(userRef);
        if (!userSnapshot.exists()) {
          // If the data doesn't exist, create it
          await set(userRef, {
            email: user.email,
            // ... any other initial data you want to set for the user ...
          });
        }

        console.log("User signed in:", user);
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