const char *irName0[] = {
  "Power","Display","OffTimer","DisplaySize","CATV","3Keta","Favorits","1","2","3","4","5","6","7","8","9","10","11","12","TiD","BS","CS","TiA","Mute","VolumeUp","VolumeDown","ChUp","ChDown","SelectInput","d","AVPos","iLink","TV/Radio","Program","Ura","ProgInfo","MENU","Up","Left","Right","Down","OK","Exit","Return","Blue","Red","Green","Yellow"
};

const int irLen0 = 15;

const uint16_t irData0[] PROGMEM = {
/* 0 */ 99, 3450,1550,500,450, 426,1189, 17476, 4420, 16469, 260, 276, 20737, 0, 0, 
/* 1 */ 99, 3450,1550,500,400, 428,1178, 17476, 4420, 16469, 260, 325, 1, 0, 0, 
/* 2 */ 99, 3450,1550,500,450, 426,1183, 17476, 4420, 16469, 260, 324, 257, 0, 0, 
/* 3 */ 99, 3450,1550,550,450, 424,1193, 17476, 4420, 16469, 260, 20753, 1025, 0, 0, 
/* 4 */ 99, 3450,1600,450,400, 418,1215, 17476, 4420, 16469, 260, 1025, 16641, 0, 0, 
/* 5 */ 99, 3400,1600,500,400, 423,1200, 17476, 4420, 16469, 260, 4420, 5124, 0, 0, 
/* 6 */ 99, 3450,1600,500,450, 430,1181, 17476, 4420, 16469, 260, 16469, 21508, 0, 0, 
/* 7 */ 99, 3500,1550,500,400, 430,1175, 17476, 4420, 16469, 260, 4180, 1284, 0, 0, 
/* 8 */ 99, 3450,1500,550,400, 427,1186, 17476, 4420, 16469, 260, 4181, 1028, 0, 0, 
/* 9 */ 99, 3400,1600,450,450, 425,1190, 17476, 4420, 16469, 260, 4352, 20484, 0, 0, 
/* 10 */ 99, 3450,1550,500,450, 429,1177, 17476, 4420, 16469, 260, 4353, 20740, 0, 0, 
/* 11 */ 99, 3450,1550,550,450, 425,1189, 17476, 4420, 16469, 260, 4356, 21508, 0, 0, 
/* 12 */ 99, 3400,1600,450,400, 422,1208, 17476, 4420, 16469, 260, 4357, 21764, 0, 0, 
/* 13 */ 99, 3500,1500,550,400, 428,1175, 17476, 4420, 16469, 260, 4368, 16388, 0, 0, 
/* 14 */ 99, 3450,1550,450,450, 424,1198, 17476, 4420, 16469, 260, 4369, 16644, 0, 0, 
/* 15 */ 99, 3450,1550,500,400, 429,1180, 17476, 4420, 16469, 260, 4372, 17412, 0, 0, 
/* 16 */ 99, 3450,1550,500,400, 427,1190, 17476, 4420, 16469, 260, 4373, 17668, 0, 0, 
/* 17 */ 99, 3450,1500,550,400, 425,1185, 17476, 4420, 16469, 260, 4416, 4100, 0, 0, 
/* 18 */ 99, 3450,1550,450,450, 423,1202, 17476, 4420, 16469, 260, 4417, 4356, 0, 0, 
/* 19 */ 99, 3400,1600,450,400, 421,1203, 17476, 4420, 16469, 260, 16449, 16388, 0, 0, 
/* 20 */ 99, 3400,1600,500,400, 425,1193, 17476, 4420, 16469, 260, 16452, 17668, 0, 0, 
/* 21 */ 99, 3450,1550,500,400, 426,1191, 17476, 4420, 16469, 260, 16453, 17412, 0, 0, 
/* 22 */ 99, 3450,1550,500,400, 424,1202, 17476, 4420, 16469, 260, 16468, 21764, 0, 0, 
/* 23 */ 99, 3400,1600,450,400, 422,1205, 17476, 4420, 16469, 260, 277, 20481, 0, 0, 
/* 24 */ 99, 3400,1650,450,400, 425,1195, 17476, 4420, 16469, 260, 272, 21761, 0, 0, 
/* 25 */ 99, 3400,1600,500,450, 423,1200, 17476, 4420, 16469, 260, 273, 21505, 0, 0, 
/* 26 */ 99, 3450,1600,450,400, 423,1198, 17476, 4420, 16469, 260, 257, 17409, 0, 0, 
/* 27 */ 99, 3450,1550,500,400, 425,1188, 17476, 4420, 16469, 260, 260, 16641, 0, 0, 
/* 28 */ 99, 3400,1600,500,450, 421,1200, 17476, 4420, 16469, 260, 261, 16385, 0, 0, 
/* 29 */ 99, 3500,1500,550,400, 428,1182, 17476, 4420, 16469, 260, 4436, 1028, 0, 0, 
/* 30 */ 99, 3450,1550,500,450, 428,1188, 17476, 4420, 16469, 260, 21525, 1281, 0, 0, 
/* 31 */ 99, 3400,1600,500,450, 424,1200, 17476, 4420, 16469, 260, 5461, 260, 0, 0, 
/* 32 */ 99, 3450,1550,500,450, 425,1191, 17476, 4420, 16469, 260, 5189, 4100, 0, 0, 
/* 33x program */ 99, 3450,1600,450,400, 428,1190, 17476, 4420, 16469, 260, 5393, 17668, 0, 0, 
/* 33 */ 99, 3450,1600,450,400, 428,1190, 17476, 4420, 16469, 260, 5393, 17668, 0, 0, 
/* 34 */ 99, 3450,1550,550,450, 429,1183, 17476, 4420, 16469, 260, 4437, 1284, 0, 0, 
/* 35 */ 99, 3450,1550,500,450, 425,1188, 17476, 4420, 16469, 260, 20496, 1025, 0, 0, 
/* 36 */ 99, 3450,1550,500,450, 428,1182, 17476, 4420, 16469, 260, 4373, 16385, 0, 0, 
/* 37 */ 99, 3450,1550,500,400, 427,1186, 17476, 4420, 16469, 260, 20757, 1, 0, 0, 
/* 38 */ 99, 3450,1550,550,450, 432,1175, 17476, 4420, 16469, 260, 20800, 21761, 0, 0, 
/* 39 */ 99, 3400,1600,500,450, 421,1194, 17476, 4420, 16469, 260, 1024, 16385, 0, 0, 
/* 40 */ 99, 3450,1550,500,450, 422,1202, 17476, 4420, 16469, 260, 21777, 1, 0, 0, 
/* 41x exit */ 99, 3450,1600,500,400, 429,1173, 17476, 4420, 16469, 260, 21520, 1, 0, 0, 
/* 41 */ 99, 3450,1600,500,400, 429,1173, 17476, 4420, 16469, 260, 21520, 1, 0, 0, 
/* 42 */ 99, 3400,1600,500,450, 421,1197, 17476, 4420, 16469, 260, 16384, 260, 0, 0, 
/* 43 */ 99, 3450,1550,550,450, 424,1181, 17476, 4420, 16469, 260, 16385, 4, 0, 0, 
/* 44 */ 99, 3450,1550,550,450, 425,1183, 17476, 4420, 16469, 260, 16388, 1284, 0, 0, 
/* 45 */ 99, 3400,1600,550,450, 424,1188, 17476, 4420, 16469, 260, 16389, 1028, 0, 0, 
};
