int separate (String& str,  // pass as a reference to avoid double memory usage
              char **p,  int size, char **pdata ) {
  int  n = 0;
  free(*pdata); // clean up last data as we are reusing CommandArray[ ]
  // BE CAREFUL not to free it twice
  // calling free on NULL is OK
  *pdata = strdup(str.c_str()); // allocate new memory for the result.
  if (*pdata == NULL) {
    Serial.println("OUT OF MEMORY");
    return 0;
  }
  *p++ = strtok (*pdata, " ");
  for (n = 1; NULL != (*p++ = strtok (NULL, " ")); n++) {
    if (size == n) {
      break;
    }
  }
  return n;
}
// use this free the memory after use
void freeData(char **pdata) {
  free(*pdata);
  *pdata = NULL;
//  numberOfStr = 0;
}
