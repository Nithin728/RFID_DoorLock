<template>
  <!-- Main container for displaying keycodes and access information -->
  <div class="keycodes-container">
    <h2 class="title">View Log</h2>
    <!-- Section for displaying access status and times -->
    <div class="access-info">
      <p>Access Status: <span class="status">{{ accessStatus }}</span></p>
      <p>Access Granted Time: <span class="grantedTime">{{ accessGrantedTime }}</span></p>
      <p>Access Denied Time: <span class="deniedTime">{{ accessDeniedTime }}</span></p>
    </div>
    <!-- List of keycodes -->
    <ul class="keycodes-list">
      <!-- Loop through each keycode and display its content and date -->
      <li v-for="(keycode, keycodeId) in keycodes" :key="keycodeId" class="keycode-item">
        <span class="content">Content: {{ keycode.content }}</span> - 
        <span class="date">Date: {{ new Date(keycode.date).toLocaleString() }}</span>
      </li>
    </ul>
  </div>
</template>

<script setup>
// Importing necessary Vue and Firebase functions
import { ref, onMounted } from 'vue';
import { getDatabase, ref as dbRef, onValue } from "firebase/database";
import { getAuth } from "firebase/auth";

// Declaring reactive variables to store data from Firebase
const keycodes = ref({});
const accessStatus = ref('');
const accessGrantedTime = ref('');
const accessDeniedTime = ref('');

// Function to execute when the component is mounted
onMounted(() => {
  // Getting the current authenticated user
  const auth = getAuth();
  const user = auth.currentUser;

  // Check if a user is logged in
  if (user) {
    // Accessing the Firebase database
    const database = getDatabase();
    // References to specific paths in the Firebase database
    const userKeycodesRef = dbRef(database, 'users/' + user.uid + '/keycodes');
    const accessStatusRef = dbRef(database, 'accessStatus');
    const accessGrantedTimeRef = dbRef(database, 'accessGrantedTime');
    const accessDeniedTimeRef = dbRef(database, 'accessDeniedTime');

    // Listening for changes in the keycodes data
    onValue(userKeycodesRef, (snapshot) => {
      if (snapshot.exists()) {
        keycodes.value = snapshot.val();
      } else {
        console.log("No keycodes available");
      }
    }, (error) => {
      console.error(error);
    });

    // Listening for changes in the access status
    onValue(accessStatusRef, (snapshot) => {
      if (snapshot.exists()) {
        accessStatus.value = snapshot.val();
      }
    }, (error) => {
      console.error(error);
    });

    // Listening for changes in the access granted time
    onValue(accessGrantedTimeRef, (snapshot) => {
      if (snapshot.exists()) {
        accessGrantedTime.value = snapshot.val();
      }
    }, (error) => {
      console.error(error);
    });

    // Listening for changes in the access denied time
    onValue(accessDeniedTimeRef, (snapshot) => {
      if (snapshot.exists()) {
        accessDeniedTime.value = snapshot.val();
      }
    }, (error) => {
      console.error(error);
    });
  } else {
    console.log("User not logged in");
  }
});

</script>

<style>
/* CSS styles for various elements in the template */
.keycodes-container {
  max-width: 600px;
  margin: 0 auto;
  padding: 20px;
  text-align: center;
}

.title {
  font-size: 2rem;
  margin-bottom: 20px;
}

.keycodes-list {
  list-style-type: none;
  padding: 0;
}

.keycode-item {
  background-color: #1b191e;
  border: 1px solid #ddd;
  padding: 10px;
  margin-bottom: 10px;
  border-radius: 5px;
  font-size: 1.2rem;
}

.content {
  font-weight: bold;
}

.date {
}

.access-info {
  margin-bottom: 20px;
}

.status {
  font-weight: bold;
}

.grantedTime {
  font-weight: bold;
  font-size: 1.3rem;
}

.deniedTime {
  font-weight: bold;
  font-size: 1.3rem;
}

</style>
