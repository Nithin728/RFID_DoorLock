<!-- Keycodes.vue functionality -->

<template>
  <div class="keycodes">

    <!-- Title of the webpage -->
    <div class="title has-text-centered" style="font-size: 2rem;">
    Edit Key Codes
    </div>
    <div class="subtitle has-text-centered" style="font-size: 1rem; margin-bottom: 0rem;">
      Format: XX XX XX XX 
    </div>
    <div class="subtitle has-text-centered" style="font-size: 1rem;">
      (Only use numbers and capital letters)
    </div>


    <!-- The bar and button to enter keycodes -->
    <form
      @submit.prevent="addKeycode"
    >
      <div class="field has-addons mb-5">
        <div class="control">
          <input 
            v-model="newKeycodeContent"
            class="input"
            type="text" 
            id="textInput" 
            @input="formatInput"
            ref="textInputRef"
            maxlength="11"
            pattern="^[A-Z0-9 ]{11}$"
            placeholder="Enter a keycode"
            style="font-size: 1rem; height: 2rem;"
          >
        </div>
        <div class="control">
          <button
          :disabled="!newKeycodeContent"
           class="button is-info"
           style="font-size: 1rem; height: 2.18rem;"
          >
            Add
          </button>
        </div>
      </div>
    </form>

    <!-- Table that holds the keycodes -->
    <div class="container">
      <table id="myTable">
        <tbody>
          <tr class="mb-5" v-for="keycode in keycodes">
            <td style="font-size: 1rem;">
              {{ keycode.content }}
            </td>
            <td class="has-text-right">
              <button
                @click="deleteKeycode(keycode.id)"
                class="button is-danger"
                style="margin-left: 8rem;"
              >
                &cross;
              </button>
            </td>
          </tr>
        </tbody>
      </table>
    </div>

  </div>
</template>

<!-- Database Management for firestore-->
<!-- <script setup>

  import {ref, onMounted} from 'vue'
  import { collection, onSnapshot, addDoc, doc, deleteDoc, query, orderBy} from "firebase/firestore"
  import {db} from '@/firebase'
  import { getAuth } from "firebase/auth";


  // the collection that holds all of the keycodes
  const keycodesCollectionRef = collection(db, 'keycodes')
  const keycodesCollectionQuery = query(keycodesCollectionRef, orderBy("date", "desc"))

  // the array that holds all of the keycodes
  const keycodes = ref([
    // {
    //   id: 'id1',
    //   content: '1234'
    // },
    // {
    //   id: 'id2',
    //   content: '5678'
    // }
  ])

  //get keycodes
  // onMounted ( () => {
  //   onSnapshot(keycodesCollectionRef, (querySnapshot) => {
  //     const fbkeycodes = []
  //     querySnapshot.forEach((doc) => {
  //       const keycodes = {
  //         id: doc.id,
  //         content: doc.data().content,
  //       }
  //       fbkeycodes.push(keycodes)
  //     })
  //     keycodes.value = fbkeycodes
  //     console.log(keycodes.value)
  //   })
  // })

  onMounted(() => {
  const auth = getAuth();
  const userId = auth.currentUser.uid; // Get the authenticated user's UID

  // Reference to the keycodes collection specific to the authenticated user
  const userKeycodesCollectionRef = collection(db, 'users', userId, 'keycodes');

  onSnapshot(userKeycodesCollectionRef, (querySnapshot) => {
    const fbkeycodes = [];
    querySnapshot.forEach((doc) => {
      const keycodes = {
        id: doc.id,
        content: doc.data().content,
      }
      fbkeycodes.push(keycodes);
    });
    keycodes.value = fbkeycodes;
    console.log(keycodes.value);
    });
  });


  // whatever is typed in the input bar will show up here
  const newKeycodeContent = ref('')
  
  // adds keycodes through the input bar
  // const addKeycode = () => {

  //   addDoc(keycodesCollectionRef, {
  //     content: newKeycodeContent.value,
  //     date: Date.now()
  //   })
  //   newKeycodeContent.value=''
  // }
  
  const addKeycode = async () => {
  const auth = getAuth();
  const userId = auth.currentUser.uid; // Get the authenticated user's UID

  // Reference to the keycodes collection specific to the authenticated user
  const userKeycodesCollectionRef = collection(db, 'users', userId, 'keycodes');

  try {
    await addDoc(userKeycodesCollectionRef, {
      content: newKeycodeContent.value,
      date: Date.now()
    });
    console.log("Keycode added successfully!");
    newKeycodeContent.value = '';
    } catch (error) {
    console.error("Error adding keycode: ", error);
    }
  };

  //deletes keycodes
  // const deleteKeycode = id => {
  //   deleteDoc(doc(keycodesCollectionRef , id))
  // }

  const deleteKeycode = async id => {
  const auth = getAuth();
  const userId = auth.currentUser.uid; // Get the authenticated user's UID
  console.log("Got user ID");

  // Reference to the keycode document specific to the authenticated user
  const userKeycodeDocRef = doc(db, 'users', userId, 'keycodes', id);
  console.log("Got user keycode doc ref");

  try {
    await deleteDoc(userKeycodeDocRef);
    console.log("Keycode deleted successfully!");
  } catch (error) {
    console.error("Error deleting keycode: ", error);
  }
  console.log("Try block finished");
  };

  // Formats the input bar to have spaces every 2 characters
  const textInputRef = ref(null);

  function formatInput() {
  const input = textInputRef.value;
  let value = input.value.replace(/\s+/g, ''); // Remove any existing spaces first
  let formatted = '';

  for (let i = 0; i < value.length; i++) {
    formatted += value[i];
    if ((i + 1) % 2 === 0 && i !== value.length - 1) { // Check if it's every 2 characters
      formatted += ' '; // Add a space
    }
  }

  input.value = formatted;
  }
  
</script> -->

<!-- Database Management for RTDB-->
<script setup>
  import { ref, onMounted } from 'vue';
  import { getDatabase, ref as dbRef, push, onValue, remove, set } from "firebase/database";
  import { getAuth } from "firebase/auth";
  import { db } from '@/firebase'; // Make sure this points to your Firebase app instance

  // the array that holds all of the keycodes
  const keycodes = ref([]);

  // Reference to the Realtime Database
  const database = getDatabase();

  onMounted(() => {
    const auth = getAuth();
    const userId = auth.currentUser.uid; // Get the authenticated user's UID

    // Reference to the keycodes node specific to the authenticated user
    const userKeycodesRef = dbRef(database, 'users/' + userId + '/keycodes');

    // Listen for realtime updates
    onValue(userKeycodesRef, (snapshot) => {
      const data = snapshot.val();
      const fbkeycodes = [];
      for (let id in data) {
        fbkeycodes.push({ id, content: data[id].content });
      }
      keycodes.value = fbkeycodes;
    });
  });

  // whatever is typed in the input bar will show up here
  const newKeycodeContent = ref('');

  // adds keycodes through the input bar
  const addKeycode = async () => {
    const auth = getAuth();
    const userId = auth.currentUser.uid; // Get the authenticated user's UID

    // Reference to the keycodes node specific to the authenticated user
    const userKeycodesRef = dbRef(database, 'users/' + userId + '/keycodes');

    try {
      const newKeycodeRef = push(userKeycodesRef);
      await set(newKeycodeRef, {
        content: newKeycodeContent.value,
        date: Date.now()
      });
      newKeycodeContent.value = '';
    } catch (error) {
      console.error("Error adding keycode: ", error);
    }
  };

  // deletes keycodes
  const deleteKeycode = async id => {
    const auth = getAuth();
    const userId = auth.currentUser.uid; // Get the authenticated user's UID

    // Reference to the specific keycode node
    const keycodeRef = dbRef(database, 'users/' + userId + '/keycodes/' + id);

    try {
      await remove(keycodeRef);
    } catch (error) {
      console.error("Error deleting keycode: ", error);
    }
  };


  // Formats the input bar to have spaces every 2 characters
  const textInputRef = ref(null);

  function formatInput() {
  const input = textInputRef.value;
  let value = input.value.replace(/\s+/g, ''); // Remove any existing spaces first
  let formatted = '';

  for (let i = 0; i < value.length; i++) {
    formatted += value[i];
    if ((i + 1) % 2 === 0 && i !== value.length - 1) { // Check if it's every 2 characters
      formatted += ' '; // Add a space
    }
  }

  input.value = formatted;
  }
</script>


<!-- Using bulma to make the frontend look nicer -->
<style>
  @import 'bulma/css/bulma.min.css';

  .keycodes {
    max-width: 100rem;
    margin: 0 auto;
    padding: 1rem
  }

  .myTable {
    width: 100%;  
    margin: 0 auto;
  }

</style>