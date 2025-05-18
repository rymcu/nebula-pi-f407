#include "my_lvgl_pages.h"
#include "lvgl.h"

//static lv_obj_t *one,*two,*three,*four;
//	typedef struct my_page
//	{
//		uint32_t obj_addr;
//	} my_page_t;

//void my_lvgl_pages(void)
//{
//	one = lv_obj_create(NULL);
//	two = lv_obj_create(NULL);	
//	three = lv_obj_create(NULL);
//	four = lv_obj_create(NULL);	
//		

//	lv_ll_t* ll_test;

//	//my_page_t *page_test;
//	my_page_t *page_temp;
//	uint32_t length = 0;

//	_lv_ll_init(ll_test, sizeof(my_page_t));

//	page_temp = _lv_ll_ins_head(ll_test);
//	page_temp->obj_addr = (uint32_t)one;
//	
//	page_temp = _lv_ll_ins_head(ll_test);
//	page_temp->obj_addr = (uint32_t)two;
//	
//	page_temp = _lv_ll_ins_head(ll_test);
//	page_temp->obj_addr = (uint32_t)three;
//	
//	page_temp = _lv_ll_ins_head(ll_test);
//	page_temp->obj_addr = (uint32_t)four;
//	
//	length = _lv_ll_get_len(ll_test);
//	
//	LV_LOG_USER("length=%d\n",length);
//	LV_LOG_USER("one:%d,two:%d,three:%d,four:%d",one,two,three,four);
//	
//	page_temp = _lv_ll_get_head(ll_test);
//	
//	
//	while(page_temp)
//	{
//		static uint8_t i = 1;
//		
//		LV_LOG_USER(" node:%d,obj_addr:%d",i++,page_temp->obj_addr);
//		page_temp = _lv_ll_get_next(ll_test,page_temp);
//	}
//	
//	LV_LOG_USER(" head page_temp=%d\n",page_temp);
//	page_temp = _lv_ll_get_prev(ll_test,page_temp);
//	LV_LOG_USER("before head page_temp=%d\n",page_temp);		
//}
////�󻮷�����һ��ҳ�����
//	lv_obj_t* ry_left_page(lv_ll_t* ll_test,my_page_t* my_page)
//	{
//		if(my_page != NULL)
//		{
//			my_page_t* page_temp = _lv_ll_get_next(ll_test,my_page);
//			return (lv_obj_t*)(page_temp->obj_addr);
//		}
//		else
//			return NULL;
//	}
////�һ�����ǰһ��ҳ�����	
//	lv_obj_t* ry_right_page(lv_ll_t* ll_test,my_page_t* my_page)
//	{
//		if(my_page != NULL)
//		{
//			my_page_t* page_temp = _lv_ll_get_prev(ll_test,my_page);
//			return (lv_obj_t*)(page_temp->obj_addr);
//		}
//		else
//			return NULL;
//	}	
	
////��������ڵ�ṹ��
//typedef struct node
//{
//	lv_obj_t* obj;
//	
//	struct node *prev;
//	struct node *next;
//	
//}ry_node_t;
////�����ڵ�
//ry_node_t* ry_create_node(lv_obj_t* obj)
//{
//	ry_node_t* new_node = (ry_node_t*)lv_malloc(sizeof(ry_node_t));//��LVGLջ�ϴ����µ�node
//	
//	if(new_node)
//	{
//		new_node->obj = obj;
//		new_node->next = NULL;
//		new_node->prev = NULL;
//		
//		return new_node;
//	}
//	else
//	{
//		LV_LOG_USER("lv_malloc fail!");
//		return NULL;
//	}
//}
////��ʼ��������ͷ�ڵ�
//ry_node_t* ry_init_list(lv_obj_t* obj)
//{
//	ry_node_t* head = ry_create_node(obj);
//	
//	head->next = head;
//	head->prev = head;
//	LV_LOG_USER("init node ok!");
//	return head;
//}
////����ڵ�
//ry_node_t* ry_insert_node(ry_node_t * head,lv_obj_t* obj,uint8_t add)//���������ֱ�Ϊ��ͷ�����ݣ�λ�ã�1��ʾͷ�ڵ�λ�ã�
//{
//  ry_node_t * temp = (ry_node_t*)lv_malloc(sizeof(ry_node_t));//������ʱ�ڵ�
//  temp->obj  = obj;
//  temp->prev = NULL;
//  temp->next = NULL;
//    
//  if(add ==1) //���뵽����ͷ�����⿼��
//	{
//    temp->next = head;
//    head->prev = temp;
//    head = temp;
//  }
//	else
//	{
//    ry_node_t * body=head;//�ҵ�Ҫ����λ�õ�ǰһ�����
//        
//    for(uint8_t i=1; i<add-1; i++) 
//		{
//			if(body->next == NULL) break;	
//      body = body->next;				
//		}					        
//		if(body->next==NULL)//���������ѯ�����Ϊ�ռ�Ϊ���һ���ڵ㣬�ڵ������󲢷��ء�
//		{
//      body->next = temp;
//      temp->prev = body;
//		}
//		else
//		{
//			body->next->prev = temp;//�����м�
//			temp->next = body->next;
//			body->next = temp;
//			temp->prev = body;
//		} 
//  }
//	LV_LOG_USER("insert node ok!");
//  return head;	
//}
////ɾ���ڵ�
//ry_node_t* ry_delete_node(ry_node_t* head,lv_obj_t* obj)
//{
//	ry_node_t * body=head;//�ҵ�Ҫ����λ�õ�ǰһ�����
//	
//	while(body)
//	{
//		if(body->obj == obj)
//		{
//      body->prev->next = body->next;
//      body->next->prev = body->prev;
//      lv_free(body);
//      LV_LOG_USER("delete node ok!");
//      return head;
//		}
//		body = body->next;
//	}
//	LV_LOG_USER("delete node fail!");
//	return head;
//}

////����˫����,ͬʱ��ӡԪ������
//void ry_printf_list(ry_node_t *head)
//	{
//    ry_node_t *list = head;
//    int pos=1;
//    while(list)
//		{
//      LV_LOG_USER("%d:%d",pos++,list->obj);
//      list=list->next;
//    }
//}
//	
//static lv_obj_t *one,*two,*three,*four;
//void my_lvgl_pages(void)
//{
//	one = lv_obj_create(NULL);
//	two = lv_obj_create(NULL);	
//	three = lv_obj_create(NULL);
//	four = lv_obj_create(NULL);	
//	
//	LV_LOG_USER("one:%d,two:%d,three:%d,four:%d",one,two,three,four);
//	
//	ry_node_t* list_head = ry_init_list(one);
//	//ry_insert_node(list_head,one,1);
//	//ry_insert_node(list_head,two,1);
//	//ry_insert_node(list_head,three,1);
//	//ry_insert_node(list_head,four,1);
//	
//	//ry_printf_list(list_head);
//	//ry_delete_node(list_head,one);
//	for(uint8_t i = 0;i <1;i++)
//	{
//		ry_node_t * list=list_head;
//		LV_LOG_USER("%d--%d--%d--%d",i,list->prev,list->obj,list->next);
//		list=list->next;
//	}
//	
//	
//	
//}

