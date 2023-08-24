#include <stdio.h>
#include "mysql.h"

#pragma comment(lib, "libmysql.lib")
#pragma warning(disable : 4996)
#define MAX_QUERY 100000
const char *host = "localhost";
const char *user = "root";
const char *pw = "Dlrjsghk12@";
const char *db = "delivery";

int main(void)
{

	MYSQL *connection = NULL;
	MYSQL conn;
	MYSQL_RES *sql_result;
	MYSQL_ROW sql_row;

	if (mysql_init(&conn) == NULL)
		printf("mysql_init() error!");

	connection = mysql_real_connect(&conn, host, user, pw, db, 3306, (const char *)NULL, 0);
	if (connection == NULL)
	{
		printf("%d ERROR : %s\n", mysql_errno(&conn), mysql_error(&conn));
		return 1;
	}

	else
	{
		printf("Connection Succeed\n");
		if (mysql_select_db(&conn, db))
		{
			printf("%d ERROR : %s\n", mysql_errno(&conn), mysql_error(&conn));
			return 1;
		}
		char query[MAX_QUERY] = "";
		char line[MAX_QUERY] = "";
		int drop_need = 0;

		FILE* ddlFile = fopen("DDL.txt", "r");
		if (ddlFile == NULL)
		{
			printf("Failed to open DDL.txt\n");
			return 1;
		}
		while (fgets(line, sizeof(line), ddlFile) != NULL)
		{
			strcat(query, line);

			// Execute query if it ends with a semicolon
			if (strlen(query) > 0 && query[strlen(query) - 2] == ';')
			{
				if (mysql_query(connection, query) != 0)
				{
					drop_need = 1;
					break;
				}
				query[0] = '\0'; // Reset query
			}
		}
		fclose(ddlFile);
		if (drop_need)
		{
			FILE* ddldropFile = fopen("DDLdrop.txt", "r");
			if (ddldropFile == NULL)
			{
				printf("Failed to open DDLdrop.txt\n");
				return 1;
			}

			char query[MAX_QUERY] = "";
			char line[MAX_QUERY] = "";
			while (fgets(line, sizeof(line), ddldropFile) != NULL)
			{
				strcat(query, line);

				// Execute query if it ends with a semicolon
				if (strlen(query) > 0 && query[strlen(query) - 2] == ';')
				{
					if (mysql_query(connection, query) != 0)
					{
						printf("DDLdrop %d ERROR : %s\n", mysql_errno(connection), mysql_error(connection));
						break;
					}
					query[0] = '\0'; // Reset query
				}
			}
			fclose(ddldropFile);
		}
		query[0] = '\0';
		line[0] = '\0';
		// Execute INSERT queries from insert.txt
		FILE* insertFile = fopen("Insert.txt", "r");
		if (insertFile == NULL)
		{
			printf("Failed to open Insert.txt\n");
			return 1;
		}

		while (fgets(line, sizeof(line), insertFile) != NULL)
		{
			strcat(query, line);

			// Execute query if it ends with a semicolon
			if (strlen(query) > 0 && query[strlen(query) - 2] == ';')
			{
				if (mysql_query(connection, query) != 0)
				{
					printf("Insert %d ERROR : %s\n", mysql_errno(connection), mysql_error(connection));
					break;
				}
				query[0] = '\0'; // Reset query
			}
		}
		fclose(insertFile);
		int state;
		
		int type = 0;
		do
		{
			printf("------- SELECT QUERY TYPES -------\n\n");
			printf("\t1. TYPE 1\n");
			printf("\t2. TYPE 2\n");
			printf("\t3. TYPE 3\n");
			printf("\t4. TYPE 4\n");
			printf("\t5. TYPE 5\n");
			printf("\t0. QUIT\n");
			state = 0;
			int temp_ret = scanf("%d", &type);
			if (type == 1)
			{
				int subtype = 0;
				do
				{
					printf("------- Subtypes in TYPE 1 -------\n");
					printf("\t1. TYPE 1-1\n");
					printf("\t2. TYPE 1-2\n");
					printf("\t3. TYPE 1-3\n");
					int temp_ret = scanf("%d", &subtype);
					/* Find all customers who had a package on the truck at the time of the crash. */
					if (subtype == 1)
					{
						char when[100];	 // 2023-05-17 12:00:00
						char truck[100]; // airplane_b
						char temp;
						printf("---- TYPE 1-1 ----\n");
						printf("**Find all customers who had a package on the truck at the time of the crash.**\n");
						printf("Which Truck? ex)truck_b : ");
						int i = 0;
						while ((temp = getchar()) != EOF)
						{
							if (temp == '\n' && i != 0)
								break;
							if (temp != '\n')
								truck[i++] = temp;
						}
						truck[i] = '\0';
						printf("When? ex)2023-05-17 20:00:00 : ");
						i = 0;
						while ((temp = getchar()) != EOF)
						{
							if (temp == '\n' && i != 0)
								break;
							if (temp != '\n')
								when[i++] = temp;
						}
						when[i] = '\0';
						sprintf(query, "select distinct customer_ID_sender from carry natural join service where transportation_ID = '%s' and '%s' between carry.from_when and carry.to_when;", truck, when);

						state = mysql_query(connection, query);
						if (state == 0)
						{
							printf("[RESULT : (Customer_ID)Sender who had a package on that truck]\n");
							sql_result = mysql_store_result(connection);
							unsigned int num_rows = mysql_num_rows(sql_result);
							if (num_rows == 0)
								printf("[NONE]\n");
							while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
							{
								printf("%s ", sql_row[0]);
								printf("\n");
							}
							mysql_free_result(sql_result);
						}
					}
					/* Find all recipients who had a package on that truck at the time of the crash.*/
					else if (subtype == 2)
					{
						char when[100];	 // 2023-05-17 12:00:00
						char truck[100]; // airplane_b
						char temp;
						printf("---- TYPE 1-2 ----\n");
						printf("**Find all recipients who had a package on that truck at the time of the crash.**\n");
						printf("Which Truck? ex)truck_b : ");
						int i = 0;
						while ((temp = getchar()) != EOF)
						{
							if (temp == '\n' && i != 0)
								break;
							if (temp != '\n')
								truck[i++] = temp;
						}
						truck[i] = '\0';
						printf("When? ex)2023-05-17 20:00:00 : ");
						i = 0;
						while ((temp = getchar()) != EOF)
						{
							if (temp == '\n' && i != 0)
								break;
							if (temp != '\n')
								when[i++] = temp;
						}
						when[i] = '\0';
						sprintf(query, "select distinct customer_ID_reciever from carry natural join service where transportation_ID = '%s' and '%s' between carry.from_when and carry.to_when;", truck, when);

						state = mysql_query(connection, query);
						if (state == 0)
						{
							printf("[RESULT : (Customer_ID)Recipients who had package on that truck]\n");
							sql_result = mysql_store_result(connection);
							unsigned int num_rows = mysql_num_rows(sql_result);
							if (num_rows == 0)
								printf("[NONE]\n");
							while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
							{
								printf("%s ", sql_row[0]);
								printf("\n");
							}
							mysql_free_result(sql_result);
						}
					}
					/*Find the last successful delivery by that truck prior to the crash.*/
					else if (subtype == 3)
					{
						char when[100];	 // 2023-05-17 12:00:00
						char truck[100]; // airplane_b
						char temp;
						printf("---- TYPE 1-3 ----\n");
						printf("**Find the last successful delivery by that truck prior to the crash.**\n");
						printf("Which Truck? ex)truck_b : ");
						int i = 0;
						while ((temp = getchar()) != EOF)
						{
							if (temp == '\n' && i != 0)
								break;
							if (temp != '\n')
								truck[i++] = temp;
						}
						truck[i] = '\0';
						printf("When? ex)2023-05-17 20:00:00 : ");
						i = 0;
						while ((temp = getchar()) != EOF)
						{
							if (temp == '\n' && i != 0)
								break;
							if (temp != '\n')
								when[i++] = temp;
						}
						when[i] = '\0';
						sprintf(query, "select distinct location_ID_from from carry natural join shipment where transportation_ID = '%s' and '%s' between carry.from_when and carry.to_when	;", truck, when);

						state = mysql_query(connection, query);
						if (state == 0)
						{
							printf("[RESULT : (location_ID)Last successful delivery location]\n");
							sql_result = mysql_store_result(connection);
							unsigned int num_rows = mysql_num_rows(sql_result);
							if (num_rows == 0)
								printf("[NONE]\n");
							while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
							{
								printf("%s ", sql_row[0]);
								printf("\n");
							}
							mysql_free_result(sql_result);
						}
					}
				} while (subtype != 0);
			}
			/*Find the customer who has shipped the most packages in the past year.*/
			else if (type == 2)
			{
				char when[100];
				char temp;
				int i;
				printf("------- TYPE 2 -------\n");
				printf("**Find the customer who has shipped the most packages in the past year**\n");
				printf("Which year? : ");
				i = 0;
				while ((temp = getchar()) != EOF)
				{
					if (temp == '\n' && i != 0)
						break;
					if (temp != '\n')
						when[i++] = temp;
				}
				int current_year = atoi(when);
				int next_year = atoi(when) + 1;
				sprintf(query, "SELECT c.customer_ID, c.name, COUNT(*) AS send_count FROM service s JOIN customer c ON s.customer_ID_sender = c.customer_ID WHERE s.service_date >= '%04d-01-01' AND s.service_date < '%04d-01-01' GROUP BY c.customer_ID, c.name HAVING COUNT(*) = (SELECT MAX(send_count) FROM( SELECT COUNT(*) AS send_count FROM service WHERE service_date >= '%04d-01-01' AND service_date < '%04d-01-01' GROUP BY customer_ID_sender) AS subquery); ", current_year, next_year, current_year, next_year);
				state = mysql_query(connection, query);
				if (state == 0)
				{
					printf("[RESULT : (customer_ID, name)Customer who has shipped most]\n");
					sql_result = mysql_store_result(connection);
					unsigned int num_rows = mysql_num_rows(sql_result);
					if (num_rows == 0)
						printf("[NONE]\n");
					while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
					{
						printf("%s ", sql_row[0]);
						printf("%s ", sql_row[1]);
						printf("\n");
					}
					mysql_free_result(sql_result);
				}
			}

			/*Find the customer who has spent the most money on shipping in the past year.*/
			else if (type == 3)
			{
				printf("------- TYPE 3 -------\n");
				printf("**Find the customer who has spent the most money on shipping in the past year.**\n");
				char when[100];
				char temp;
				int i;
				printf("Which year? : ");
				i = 0;
				while ((temp = getchar()) != EOF)
				{
					if (temp == '\n' && i != 0)
						break;
					if (temp != '\n')
						when[i++] = temp;
				}
				when[i] = '\0';
				int current_year = atoi(when);
				int next_year = atoi(when) + 1;
				sprintf(query, "SELECT c.customer_ID, c.name FROM service s JOIN payment p ON s.payment_ID = p.payment_ID JOIN customer c ON p.customer_ID = c.customer_ID JOIN service_type_cost stc ON s.service_type = stc.service_type WHERE s.service_date >= '%04d-01-01' AND s.service_date < '%04d-01-01' GROUP BY c.customer_ID, c.name HAVING SUM(stc.service_cost) = ( SELECT MAX(total_payment) FROM( SELECT SUM(stc.service_cost) AS total_payment FROM service s JOIN payment p ON s.payment_ID = p.payment_ID JOIN service_type_cost stc ON s.service_type = stc.service_type WHERE s.service_date >= '%04d-01-01' AND s.service_date < '%04d-01-01' GROUP BY p.customer_ID) AS subquery);", current_year, next_year, current_year, next_year);
				state = mysql_query(connection, query);
				if (state == 0)
				{
					printf("[RESULT : (customer_ID, name)Customer who has spent the most money]\n");
					sql_result = mysql_store_result(connection);
					unsigned int num_rows = mysql_num_rows(sql_result);
					if (num_rows == 0)
						printf("[NONE]\n");
					while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
					{
						printf("%s ", sql_row[0]);
						printf("%s", sql_row[1]);
						printf("\n");
					}
					mysql_free_result(sql_result);
				}
			}

			/*Find those packages that were not delivered within the promised time.*/
			else if (type == 4)
			{
				printf("------- TYPE 4 -------\n");
				printf("**Find those packages that were not delivered within the promised time.**\n");
				query[0] = '\0';
				sprintf(query, "SELECT p.package_ID FROM package p JOIN service s ON p.package_ID = s.package_ID JOIN(SELECT package_ID, MAX(to_when) AS latest_locates_time FROM locates GROUP BY package_ID) l ON p.package_ID = l.package_ID WHERE l.latest_locates_time > s.promised_time;");
				state = mysql_query(connection, query);
				if (state == 0)
				{
					printf("[RESULT : (package_ID)pacakages not delivered within the promised time]\n");
					sql_result = mysql_store_result(connection);
					unsigned int num_rows = mysql_num_rows(sql_result);
					if (num_rows == 0)
						printf("[NONE]\n");
					while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
					{
						printf("%s ", sql_row[0]);
						printf("\n");
					}
					mysql_free_result(sql_result);
				}
			}

			/*Generate the bill for each customer for the past month. Consider creating several types of bills.*/
			if (type == 5)
			{
				char year[100];
				char month[100];
				int i;
				char temp;
				printf("------- TYPE 5 -------\n");
				printf("**Generate the bill for each customer for the past month. Consider creating several types of bills.**\n");
				printf("Which year? : ");
				i = 0;
				while ((temp = getchar()) != EOF)
				{
					if (temp == '\n' && i != 0)
						break;
					if (temp != '\n')
						year[i++] = temp;
				}
				year[i] = '\0';
				printf("Which month? : ");
				i = 0;
				while ((temp = getchar()) != EOF)
				{
					if (temp == '\n' && i != 0)
						break;
					if (temp != '\n')
						month[i++] = temp;
				}
				month[i] = '\0';
				int cur_year = atoi(year);
				int cur_month = atoi(month);
				int next_month = cur_month + 1;
				char bill_type[100];
				int bill;
				printf("[RESULT : bills]\n");
				for (int i = 1; i < 3; i++)
				{
					bill = i;
					if (bill == 1)
						strcpy(bill_type, "monthly_pay");
					else if (bill == 2)
						strcpy(bill_type, "credit_card");
					sprintf(query, "SELECT c.*, p.*, s.* FROM payment p NATURAL JOIN service s NATURAL JOIN customer c WHERE p.payment_type = '%s' AND s.service_date >= '%04d-%02d-01' AND s.service_date < '%04d-%02d-01';", bill_type, cur_year, cur_month, cur_year, next_month);
					state = mysql_query(connection, query);
					if (state == 0)
					{
						printf("<bill type : %s>\n", bill_type);
						sql_result = mysql_store_result(connection);
						unsigned int num_rows = mysql_num_rows(sql_result);
						if (num_rows == 0)
							printf("[NONE]\n");
						char current_customer[100] = "";

						while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
						{
							if (strcmp(current_customer, sql_row[0]) != 0)
							{
								if (current_customer[0] != '\0')
									printf("\n");
								for (unsigned int i = 0; i < 5; i++)
								{
									if (i == 0)
										printf("customer_ID : %-10s\n", sql_row[i]);
									if (i == 1)
										printf("name : %-10s\n", sql_row[i]);
									if (i == 2 && bill == 1)
										printf("account : %-10s\n", sql_row[i]);
									if (i == 3)
										printf("phone number : %-10s\n", sql_row[i]);
									if (i == 4)
										printf("address : %-10s\n", sql_row[i]);
								}
								printf("[payment_ID]   [payment_time]      [prepaid?]   [service_ID]  [type]        [status]      [manager]     [sender]      [reciever]   [package_ID]\n");
								strcpy(current_customer, sql_row[0]);
							}
							for (unsigned int i = 5; i < mysql_num_fields(sql_result); i++)
							{
								if (i != 6 && i != 9 && i != 11 && i != 12 && i != 16)
								{
									if (i == 8)
									{
										if (strcmp(sql_row[i], "0") == 0)
											printf(" not prepaid ");
										else
											printf(" prepaid     ");
									}
									else
										printf(" %-13s", sql_row[i]);
								}
							}
							printf("\n");
						}
					}
					printf("\n\n");
				}
			}
		} while (type != 0);
		
		mysql_close(connection);
	}
		

	return 0;
}

/*
show table start
printf("-------service-------\n");
const char* temp_query = "select * from service";
int temp_state = mysql_query(connection, temp_query);
if (temp_state == 0)
{
	sql_result = mysql_store_result(connection);
	unsigned int num_rows = mysql_num_rows(sql_result);
	printf("Number of rows: %u\n", num_rows);

	while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
	{
		for (unsigned int i = 0; i < mysql_num_fields(sql_result); i++)
		{
			printf("%s ", sql_row[i]);
		}
		printf("\n");
	}

	mysql_free_result(sql_result);
}

printf("-------customer-------\n");
temp_query = "select * from customer";
temp_state = mysql_query(connection, temp_query);
if (temp_state == 0)
{
	sql_result = mysql_store_result(connection);
	unsigned int num_rows = mysql_num_rows(sql_result);
	printf("Number of rows: %u\n", num_rows);

	while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
	{
		for (unsigned int i = 0; i < mysql_num_fields(sql_result); i++)
		{
			printf("%s ", sql_row[i]);
		}
		printf("\n");
	}

	mysql_free_result(sql_result);
}

printf("-------payment-------\n");
temp_query = "select * from payment";
temp_state = mysql_query(connection, temp_query);
if (temp_state == 0)
{
	sql_result = mysql_store_result(connection);
	unsigned int num_rows = mysql_num_rows(sql_result);
	printf("Number of rows: %u\n", num_rows);

	while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
	{
		for (unsigned int i = 0; i < mysql_num_fields(sql_result); i++)
		{
			printf("%s ", sql_row[i]);
		}
		printf("\n");
	}

	mysql_free_result(sql_result);
}

printf("-------service_type_cost-------\n");
temp_query = "select * from service_type_cost";
temp_state = mysql_query(connection, temp_query);
if (temp_state == 0)
{
	sql_result = mysql_store_result(connection);
	unsigned int num_rows = mysql_num_rows(sql_result);
	printf("Number of rows: %u\n", num_rows);

	while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
	{
		for (unsigned int i = 0; i < mysql_num_fields(sql_result); i++)
		{
			printf("%s ", sql_row[i]);
		}
		printf("\n");
	}

	mysql_free_result(sql_result);
}

printf("-------shipment_track-------\n");
temp_query = "select * from shipment_track";
temp_state = mysql_query(connection, temp_query);
if (temp_state == 0)
{
	sql_result = mysql_store_result(connection);
	unsigned int num_rows = mysql_num_rows(sql_result);
	printf("Number of rows: %u\n", num_rows);

	while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
	{
		for (unsigned int i = 0; i < mysql_num_fields(sql_result); i++)
		{
			printf("%s ", sql_row[i]);
		}
		printf("\n");
	}

	mysql_free_result(sql_result);
}

printf("-------transportation-------\n");
temp_query = "select * from transportation";
temp_state = mysql_query(connection, temp_query);
if (temp_state == 0)
{
	sql_result = mysql_store_result(connection);
	unsigned int num_rows = mysql_num_rows(sql_result);
	printf("Number of rows: %u\n", num_rows);

	while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
	{
		for (unsigned int i = 0; i < mysql_num_fields(sql_result); i++)
		{
			printf("%s ", sql_row[i]);
		}
		printf("\n");
	}

	mysql_free_result(sql_result);
}

printf("-------location-------\n");
temp_query = "select * from location";
temp_state = mysql_query(connection, temp_query);
if (temp_state == 0)
{
	sql_result = mysql_store_result(connection);
	unsigned int num_rows = mysql_num_rows(sql_result);
	printf("Number of rows: %u\n", num_rows);

	while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
	{
		for (unsigned int i = 0; i < mysql_num_fields(sql_result); i++)
		{
			printf("%s ", sql_row[i]);
		}
		printf("\n");
	}

	mysql_free_result(sql_result);
}
printf("-------carry-------\n");
temp_query = "select * from carry";
temp_state = mysql_query(connection, temp_query);
if (temp_state == 0)
{
	sql_result = mysql_store_result(connection);
	unsigned int num_rows = mysql_num_rows(sql_result);
	printf("Number of rows: %u\n", num_rows);

	while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
	{
		for (unsigned int i = 0; i < mysql_num_fields(sql_result); i++)
		{
			printf("%s ", sql_row[i]);
		}
		printf("\n");
	}

	mysql_free_result(sql_result);
}
printf("-------locates-------\n");
temp_query = "select * from locates";
temp_state = mysql_query(connection, temp_query);
if (temp_state == 0)
{
	sql_result = mysql_store_result(connection);
	unsigned int num_rows = mysql_num_rows(sql_result);
	printf("Number of rows: %u\n", num_rows);

	while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
	{
		for (unsigned int i = 0; i < mysql_num_fields(sql_result); i++)
		{
			printf("%s ", sql_row[i]);
		}
		printf("\n");
	}

	mysql_free_result(sql_result);
}
printf("-------shipment-------\n");
temp_query = "select * from shipment";
temp_state = mysql_query(connection, temp_query);
if (temp_state == 0)
{
	sql_result = mysql_store_result(connection);
	unsigned int num_rows = mysql_num_rows(sql_result);
	printf("Number of rows: %u\n", num_rows);

	while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
	{
		for (unsigned int i = 0; i < mysql_num_fields(sql_result); i++)
		{
			printf("%s ", sql_row[i]);
		}
		printf("\n");
	}

	mysql_free_result(sql_result);
}
printf("-------package-------\n");
temp_query = "select * from package";
temp_state = mysql_query(connection, temp_query);
if (temp_state == 0)
{
	sql_result = mysql_store_result(connection);
	unsigned int num_rows = mysql_num_rows(sql_result);
	printf("Number of rows: %u\n", num_rows);

	while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
	{
		for (unsigned int i = 0; i < mysql_num_fields(sql_result); i++)
		{
			printf("%s ", sql_row[i]);
		}
		printf("\n");
	}

	mysql_free_result(sql_result);
}
printf("-------hazardous-------\n");
temp_query = "select * from hazardous";
temp_state = mysql_query(connection, temp_query);
if (temp_state == 0)
{
	sql_result = mysql_store_result(connection);
	unsigned int num_rows = mysql_num_rows(sql_result);
	printf("Number of rows: %u\n", num_rows);

	while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
	{
		for (unsigned int i = 0; i < mysql_num_fields(sql_result); i++)
		{
			printf("%s ", sql_row[i]);
		}
		printf("\n");
	}

	mysql_free_result(sql_result);
}
printf("-------international-------\n");
temp_query = "select * from international";
temp_state = mysql_query(connection, temp_query);
if (temp_state == 0)
{
	sql_result = mysql_store_result(connection);
	unsigned int num_rows = mysql_num_rows(sql_result);
	printf("Number of rows: %u\n", num_rows);

	while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
	{
		for (unsigned int i = 0; i < mysql_num_fields(sql_result); i++)
		{
			printf("%s ", sql_row[i]);
		}
		printf("\n");
	}

	mysql_free_result(sql_result);
}
show table done

	printf("\t6. TYPE 6\n");
	printf("\t7. TYPE 7\n");

	//printf("----------------------------------\n");
	printf("\n\n");
	printf("---- TYPE 5 ----\n\n");
	printf("** Find the top k brands by unit sales by the year**\n");
	printf(" Which K? : 3\n");

	return 0;

}
*/