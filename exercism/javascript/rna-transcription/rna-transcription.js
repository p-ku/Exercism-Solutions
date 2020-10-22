const pairs = {
  G: "C",
  C: "G",
  T: "A",
  A: "U",
  "": "",
};
export const toRna = (strand) => {
  return strand
    .split("")
    .map((nucleotide) => pairs[nucleotide])
    .join("");
};
