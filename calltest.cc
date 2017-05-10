void noret() {
}

int iret() {
  return 0;
}

void itake(int val) {
}

int main() {
  noret();
  iret();

  int a = iret();
  itake(iret());

  return iret();
}
