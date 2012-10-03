#pragma once


namespace ArmGUI {
	
	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	/// <summary>
	/// Summary for Form1
	/// </summary>

	// The OWIO 535 USB Arm
	OWI535 testArm;

	public ref class Form1 : public System::Windows::Forms::Form
	{
	public:
		Form1(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}
		
		
	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~Form1()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Button^  button1;
	private: System::Windows::Forms::Button^  button2;
	private: System::Windows::Forms::Button^  button3;
	private: System::Windows::Forms::Button^  button4;
	private: System::Windows::Forms::Button^  button5;
	private: System::Windows::Forms::Button^  button6;
	private: System::Windows::Forms::Button^  button7;
	private: System::Windows::Forms::Button^  button8;
	private: System::Windows::Forms::Button^  button9;
	private: System::Windows::Forms::Button^  button10;
	private: System::Windows::Forms::Button^  button11;
	private: System::Windows::Forms::Button^  button12;
	private: System::Windows::Forms::TextBox^  textBox1;
	private: System::Windows::Forms::Timer^  timer1;
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::CheckBox^  checkBoxUsbEnabled;
	private: System::Windows::Forms::ProgressBar^  progressBarCogPower;
	private: System::Windows::Forms::GroupBox^  groupBox1;
	private: System::Windows::Forms::GroupBox^  groupBox2;
	private: System::Windows::Forms::Label^  label5;
	private: System::Windows::Forms::Label^  label4;
	private: System::Windows::Forms::Label^  label3;
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::NumericUpDown^  numericUpDown1;
	private: System::Windows::Forms::Label^  label6;
	private: System::Windows::Forms::Label^  lblCQP8;
	private: System::Windows::Forms::Label^  lblCQP7;
	private: System::Windows::Forms::Label^  lblCQO2;
	private: System::Windows::Forms::Label^  lblCQO1;
	private: System::Windows::Forms::Label^  lblHeadSetOn;


	private: System::Windows::Forms::GroupBox^  groupBox3;


	private: System::Windows::Forms::Label^  label9;
	private: System::Windows::Forms::Label^  lblCurrentAction;
	private: System::Windows::Forms::Label^  label7;
	private: System::Windows::Forms::Label^  lblCognitivIsActive;



	private: System::ComponentModel::IContainer^  components;

	protected: 

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>

		
#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->button2 = (gcnew System::Windows::Forms::Button());
			this->button3 = (gcnew System::Windows::Forms::Button());
			this->button4 = (gcnew System::Windows::Forms::Button());
			this->button5 = (gcnew System::Windows::Forms::Button());
			this->button6 = (gcnew System::Windows::Forms::Button());
			this->button7 = (gcnew System::Windows::Forms::Button());
			this->button8 = (gcnew System::Windows::Forms::Button());
			this->button9 = (gcnew System::Windows::Forms::Button());
			this->button10 = (gcnew System::Windows::Forms::Button());
			this->button11 = (gcnew System::Windows::Forms::Button());
			this->button12 = (gcnew System::Windows::Forms::Button());
			this->textBox1 = (gcnew System::Windows::Forms::TextBox());
			this->timer1 = (gcnew System::Windows::Forms::Timer(this->components));
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->checkBoxUsbEnabled = (gcnew System::Windows::Forms::CheckBox());
			this->progressBarCogPower = (gcnew System::Windows::Forms::ProgressBar());
			this->groupBox1 = (gcnew System::Windows::Forms::GroupBox());
			this->groupBox2 = (gcnew System::Windows::Forms::GroupBox());
			this->lblHeadSetOn = (gcnew System::Windows::Forms::Label());
			this->label6 = (gcnew System::Windows::Forms::Label());
			this->lblCQP8 = (gcnew System::Windows::Forms::Label());
			this->lblCQP7 = (gcnew System::Windows::Forms::Label());
			this->lblCQO2 = (gcnew System::Windows::Forms::Label());
			this->lblCQO1 = (gcnew System::Windows::Forms::Label());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->numericUpDown1 = (gcnew System::Windows::Forms::NumericUpDown());
			this->groupBox3 = (gcnew System::Windows::Forms::GroupBox());
			this->lblCognitivIsActive = (gcnew System::Windows::Forms::Label());
			this->label9 = (gcnew System::Windows::Forms::Label());
			this->lblCurrentAction = (gcnew System::Windows::Forms::Label());
			this->label7 = (gcnew System::Windows::Forms::Label());
			this->groupBox1->SuspendLayout();
			this->groupBox2->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericUpDown1))->BeginInit();
			this->groupBox3->SuspendLayout();
			this->SuspendLayout();
			// 
			// button1
			// 
			this->button1->Enabled = false;
			this->button1->Location = System::Drawing::Point(6, 15);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(118, 23);
			this->button1->TabIndex = 0;
			this->button1->Text = L"Base Clockwise";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &Form1::button1_Click);
			this->button1->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &Form1::button1_MouseDown);
			this->button1->MouseUp += gcnew System::Windows::Forms::MouseEventHandler(this, &Form1::button1_MouseUp);
			// 
			// button2
			// 
			this->button2->Enabled = false;
			this->button2->Location = System::Drawing::Point(6, 50);
			this->button2->Name = L"button2";
			this->button2->Size = System::Drawing::Size(118, 23);
			this->button2->TabIndex = 1;
			this->button2->Text = L"Base CntrClockwise";
			this->button2->UseVisualStyleBackColor = true;
			this->button2->Click += gcnew System::EventHandler(this, &Form1::button2_Click);
			this->button2->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &Form1::button2_MouseDown);
			this->button2->MouseUp += gcnew System::Windows::Forms::MouseEventHandler(this, &Form1::button2_MouseUp);
			// 
			// button3
			// 
			this->button3->Enabled = false;
			this->button3->Location = System::Drawing::Point(124, 15);
			this->button3->Name = L"button3";
			this->button3->Size = System::Drawing::Size(118, 23);
			this->button3->TabIndex = 2;
			this->button3->Text = L"Shoulder Up";
			this->button3->UseVisualStyleBackColor = true;
			this->button3->Click += gcnew System::EventHandler(this, &Form1::button3_Click);
			this->button3->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &Form1::button3_MouseDown);
			this->button3->MouseUp += gcnew System::Windows::Forms::MouseEventHandler(this, &Form1::button3_MouseUp);
			// 
			// button4
			// 
			this->button4->Enabled = false;
			this->button4->Location = System::Drawing::Point(124, 50);
			this->button4->Name = L"button4";
			this->button4->Size = System::Drawing::Size(118, 23);
			this->button4->TabIndex = 3;
			this->button4->Text = L"Shoulder Down";
			this->button4->UseVisualStyleBackColor = true;
			this->button4->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &Form1::button4_MouseDown);
			this->button4->MouseUp += gcnew System::Windows::Forms::MouseEventHandler(this, &Form1::button4_MouseUp);
			// 
			// button5
			// 
			this->button5->Enabled = false;
			this->button5->Location = System::Drawing::Point(242, 15);
			this->button5->Name = L"button5";
			this->button5->Size = System::Drawing::Size(118, 23);
			this->button5->TabIndex = 4;
			this->button5->Text = L"Elbow &Up";
			this->button5->UseVisualStyleBackColor = true;
			this->button5->Click += gcnew System::EventHandler(this, &Form1::button5_Click);
			this->button5->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &Form1::button5_MouseDown);
			this->button5->MouseUp += gcnew System::Windows::Forms::MouseEventHandler(this, &Form1::button5_MouseUp);
			// 
			// button6
			// 
			this->button6->Enabled = false;
			this->button6->Location = System::Drawing::Point(242, 50);
			this->button6->Name = L"button6";
			this->button6->Size = System::Drawing::Size(118, 23);
			this->button6->TabIndex = 5;
			this->button6->Text = L"Elbow Down";
			this->button6->UseVisualStyleBackColor = true;
			this->button6->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &Form1::button6_MouseDown);
			this->button6->MouseUp += gcnew System::Windows::Forms::MouseEventHandler(this, &Form1::button6_MouseUp);
			// 
			// button7
			// 
			this->button7->Enabled = false;
			this->button7->Location = System::Drawing::Point(360, 15);
			this->button7->Name = L"button7";
			this->button7->Size = System::Drawing::Size(118, 23);
			this->button7->TabIndex = 6;
			this->button7->Text = L"Wrist Up";
			this->button7->UseVisualStyleBackColor = true;
			this->button7->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &Form1::button7_MouseDown);
			this->button7->MouseUp += gcnew System::Windows::Forms::MouseEventHandler(this, &Form1::button7_MouseUp);
			// 
			// button8
			// 
			this->button8->Enabled = false;
			this->button8->Location = System::Drawing::Point(360, 50);
			this->button8->Name = L"button8";
			this->button8->Size = System::Drawing::Size(118, 23);
			this->button8->TabIndex = 7;
			this->button8->Text = L"Wrist Down";
			this->button8->UseVisualStyleBackColor = true;
			this->button8->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &Form1::button8_MouseDown);
			this->button8->MouseUp += gcnew System::Windows::Forms::MouseEventHandler(this, &Form1::button8_MouseUp);
			// 
			// button9
			// 
			this->button9->Enabled = false;
			this->button9->Location = System::Drawing::Point(478, 15);
			this->button9->Name = L"button9";
			this->button9->Size = System::Drawing::Size(118, 23);
			this->button9->TabIndex = 8;
			this->button9->Text = L"Grip Close";
			this->button9->UseVisualStyleBackColor = true;
			this->button9->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &Form1::button9_MouseDown);
			this->button9->MouseUp += gcnew System::Windows::Forms::MouseEventHandler(this, &Form1::button9_MouseUp);
			// 
			// button10
			// 
			this->button10->Enabled = false;
			this->button10->Location = System::Drawing::Point(478, 50);
			this->button10->Name = L"button10";
			this->button10->Size = System::Drawing::Size(118, 23);
			this->button10->TabIndex = 9;
			this->button10->Text = L"Grip Open";
			this->button10->UseVisualStyleBackColor = true;
			this->button10->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &Form1::button10_MouseDown);
			this->button10->MouseUp += gcnew System::Windows::Forms::MouseEventHandler(this, &Form1::button10_MouseUp);
			// 
			// button11
			// 
			this->button11->Enabled = false;
			this->button11->Location = System::Drawing::Point(596, 15);
			this->button11->Name = L"button11";
			this->button11->Size = System::Drawing::Size(118, 23);
			this->button11->TabIndex = 10;
			this->button11->Text = L"Light On";
			this->button11->UseVisualStyleBackColor = true;
			this->button11->Click += gcnew System::EventHandler(this, &Form1::button11_Click);
			// 
			// button12
			// 
			this->button12->Enabled = false;
			this->button12->Location = System::Drawing::Point(596, 50);
			this->button12->Name = L"button12";
			this->button12->Size = System::Drawing::Size(118, 23);
			this->button12->TabIndex = 11;
			this->button12->Text = L"Light Off";
			this->button12->UseVisualStyleBackColor = true;
			this->button12->Click += gcnew System::EventHandler(this, &Form1::button12_Click_1);
			// 
			// textBox1
			// 
			this->textBox1->Location = System::Drawing::Point(758, 15);
			this->textBox1->Name = L"textBox1";
			this->textBox1->Size = System::Drawing::Size(35, 20);
			this->textBox1->TabIndex = 12;
			this->textBox1->TextChanged += gcnew System::EventHandler(this, &Form1::textBox1_TextChanged);
			this->textBox1->KeyDown += gcnew System::Windows::Forms::KeyEventHandler(this, &Form1::textBox1_KeyDown);
			this->textBox1->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &Form1::textBox1_KeyPress);
			// 
			// timer1
			// 
			this->timer1->Enabled = true;
			this->timer1->Tick += gcnew System::EventHandler(this, &Form1::timer1_Tick);
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(758, -1);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(35, 13);
			this->label1->TabIndex = 13;
			this->label1->Text = L"label1";
			// 
			// checkBoxUsbEnabled
			// 
			this->checkBoxUsbEnabled->AutoSize = true;
			this->checkBoxUsbEnabled->Location = System::Drawing::Point(6, 80);
			this->checkBoxUsbEnabled->Name = L"checkBoxUsbEnabled";
			this->checkBoxUsbEnabled->Size = System::Drawing::Size(111, 17);
			this->checkBoxUsbEnabled->TabIndex = 14;
			this->checkBoxUsbEnabled->Text = L"USB Arm Enabled";
			this->checkBoxUsbEnabled->UseVisualStyleBackColor = true;
			this->checkBoxUsbEnabled->CheckedChanged += gcnew System::EventHandler(this, &Form1::checkBox1_CheckedChanged);
			// 
			// progressBarCogPower
			// 
			this->progressBarCogPower->Location = System::Drawing::Point(6, 61);
			this->progressBarCogPower->Name = L"progressBarCogPower";
			this->progressBarCogPower->Size = System::Drawing::Size(185, 23);
			this->progressBarCogPower->Step = 1;
			this->progressBarCogPower->Style = System::Windows::Forms::ProgressBarStyle::Continuous;
			this->progressBarCogPower->TabIndex = 15;
			// 
			// groupBox1
			// 
			this->groupBox1->Controls->Add(this->button2);
			this->groupBox1->Controls->Add(this->button1);
			this->groupBox1->Controls->Add(this->checkBoxUsbEnabled);
			this->groupBox1->Controls->Add(this->button3);
			this->groupBox1->Controls->Add(this->button4);
			this->groupBox1->Controls->Add(this->button5);
			this->groupBox1->Controls->Add(this->button12);
			this->groupBox1->Controls->Add(this->button6);
			this->groupBox1->Controls->Add(this->button11);
			this->groupBox1->Controls->Add(this->button7);
			this->groupBox1->Controls->Add(this->button10);
			this->groupBox1->Controls->Add(this->button8);
			this->groupBox1->Controls->Add(this->button9);
			this->groupBox1->Location = System::Drawing::Point(12, 12);
			this->groupBox1->Name = L"groupBox1";
			this->groupBox1->Size = System::Drawing::Size(726, 100);
			this->groupBox1->TabIndex = 16;
			this->groupBox1->TabStop = false;
			this->groupBox1->Text = L"OWI 535 Controls";
			// 
			// groupBox2
			// 
			this->groupBox2->Controls->Add(this->lblHeadSetOn);
			this->groupBox2->Controls->Add(this->label6);
			this->groupBox2->Controls->Add(this->lblCQP8);
			this->groupBox2->Controls->Add(this->lblCQP7);
			this->groupBox2->Controls->Add(this->lblCQO2);
			this->groupBox2->Controls->Add(this->lblCQO1);
			this->groupBox2->Controls->Add(this->label5);
			this->groupBox2->Controls->Add(this->label4);
			this->groupBox2->Controls->Add(this->label3);
			this->groupBox2->Controls->Add(this->label2);
			this->groupBox2->Location = System::Drawing::Point(12, 118);
			this->groupBox2->Name = L"groupBox2";
			this->groupBox2->Size = System::Drawing::Size(263, 153);
			this->groupBox2->TabIndex = 17;
			this->groupBox2->TabStop = false;
			this->groupBox2->Text = L"Emotiv Headset Status";
			this->groupBox2->Enter += gcnew System::EventHandler(this, &Form1::groupBox2_Enter);
			// 
			// lblHeadSetOn
			// 
			this->lblHeadSetOn->AutoSize = true;
			this->lblHeadSetOn->Location = System::Drawing::Point(110, 110);
			this->lblHeadSetOn->Name = L"lblHeadSetOn";
			this->lblHeadSetOn->Size = System::Drawing::Size(98, 13);
			this->lblHeadSetOn->TabIndex = 9;
			this->lblHeadSetOn->Text = L"P8 Contact Qaulity:";
			// 
			// label6
			// 
			this->label6->AutoSize = true;
			this->label6->Location = System::Drawing::Point(6, 110);
			this->label6->Name = L"label6";
			this->label6->Size = System::Drawing::Size(73, 13);
			this->label6->TabIndex = 8;
			this->label6->Text = L"Headset On\?:";
			this->label6->Click += gcnew System::EventHandler(this, &Form1::label6_Click);
			// 
			// lblCQP8
			// 
			this->lblCQP8->AutoSize = true;
			this->lblCQP8->Location = System::Drawing::Point(110, 85);
			this->lblCQP8->Name = L"lblCQP8";
			this->lblCQP8->Size = System::Drawing::Size(98, 13);
			this->lblCQP8->TabIndex = 7;
			this->lblCQP8->Text = L"P8 Contact Qaulity:";
			// 
			// lblCQP7
			// 
			this->lblCQP7->AutoSize = true;
			this->lblCQP7->Location = System::Drawing::Point(110, 61);
			this->lblCQP7->Name = L"lblCQP7";
			this->lblCQP7->Size = System::Drawing::Size(98, 13);
			this->lblCQP7->TabIndex = 6;
			this->lblCQP7->Text = L"P7 Contact Qaulity:";
			// 
			// lblCQO2
			// 
			this->lblCQO2->AutoSize = true;
			this->lblCQO2->Location = System::Drawing::Point(110, 38);
			this->lblCQO2->Name = L"lblCQO2";
			this->lblCQO2->Size = System::Drawing::Size(94, 13);
			this->lblCQO2->TabIndex = 5;
			this->lblCQO2->Text = L"02 Contact Qaulity";
			// 
			// lblCQO1
			// 
			this->lblCQO1->AutoSize = true;
			this->lblCQO1->Location = System::Drawing::Point(110, 16);
			this->lblCQO1->Name = L"lblCQO1";
			this->lblCQO1->Size = System::Drawing::Size(97, 13);
			this->lblCQO1->TabIndex = 4;
			this->lblCQO1->Text = L"01 Contact Qaulity:";
			// 
			// label5
			// 
			this->label5->AutoSize = true;
			this->label5->Location = System::Drawing::Point(6, 85);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(98, 13);
			this->label5->TabIndex = 3;
			this->label5->Text = L"P8 Contact Qaulity:";
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Location = System::Drawing::Point(6, 61);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(98, 13);
			this->label4->TabIndex = 2;
			this->label4->Text = L"P7 Contact Qaulity:";
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(6, 38);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(94, 13);
			this->label3->TabIndex = 1;
			this->label3->Text = L"02 Contact Qaulity";
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(6, 16);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(97, 13);
			this->label2->TabIndex = 0;
			this->label2->Text = L"01 Contact Qaulity:";
			// 
			// numericUpDown1
			// 
			this->numericUpDown1->Location = System::Drawing::Point(504, 126);
			this->numericUpDown1->Name = L"numericUpDown1";
			this->numericUpDown1->Size = System::Drawing::Size(120, 20);
			this->numericUpDown1->TabIndex = 18;
			this->numericUpDown1->ValueChanged += gcnew System::EventHandler(this, &Form1::numericUpDown1_ValueChanged);
			// 
			// groupBox3
			// 
			this->groupBox3->Controls->Add(this->lblCognitivIsActive);
			this->groupBox3->Controls->Add(this->label9);
			this->groupBox3->Controls->Add(this->lblCurrentAction);
			this->groupBox3->Controls->Add(this->label7);
			this->groupBox3->Controls->Add(this->progressBarCogPower);
			this->groupBox3->Location = System::Drawing::Point(281, 118);
			this->groupBox3->Name = L"groupBox3";
			this->groupBox3->Size = System::Drawing::Size(200, 98);
			this->groupBox3->TabIndex = 19;
			this->groupBox3->TabStop = false;
			this->groupBox3->Text = L"Cognitive";
			// 
			// lblCognitivIsActive
			// 
			this->lblCognitivIsActive->AutoSize = true;
			this->lblCognitivIsActive->Location = System::Drawing::Point(91, 15);
			this->lblCognitivIsActive->Name = L"lblCognitivIsActive";
			this->lblCognitivIsActive->Size = System::Drawing::Size(35, 13);
			this->lblCognitivIsActive->TabIndex = 17;
			this->lblCognitivIsActive->Text = L"label8";
			// 
			// label9
			// 
			this->label9->AutoSize = true;
			this->label9->Location = System::Drawing::Point(6, 38);
			this->label9->Name = L"label9";
			this->label9->Size = System::Drawing::Size(77, 13);
			this->label9->TabIndex = 16;
			this->label9->Text = L"Current Action:";
			this->label9->Click += gcnew System::EventHandler(this, &Form1::label9_Click);
			// 
			// lblCurrentAction
			// 
			this->lblCurrentAction->AutoSize = true;
			this->lblCurrentAction->Location = System::Drawing::Point(88, 38);
			this->lblCurrentAction->Name = L"lblCurrentAction";
			this->lblCurrentAction->Size = System::Drawing::Size(77, 13);
			this->lblCurrentAction->TabIndex = 2;
			this->lblCurrentAction->Text = L"Current Action:";
			// 
			// label7
			// 
			this->label7->AutoSize = true;
			this->label7->Location = System::Drawing::Point(6, 16);
			this->label7->Name = L"label7";
			this->label7->Size = System::Drawing::Size(81, 13);
			this->label7->TabIndex = 1;
			this->label7->Text = L"Cognitiv Active:";
			// 
			// Form1
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(922, 329);
			this->Controls->Add(this->groupBox3);
			this->Controls->Add(this->numericUpDown1);
			this->Controls->Add(this->groupBox2);
			this->Controls->Add(this->groupBox1);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->textBox1);
			this->KeyPreview = true;
			this->Name = L"Form1";
			this->Text = L"Form1";
			this->Load += gcnew System::EventHandler(this, &Form1::Form1_Load);
			this->KeyDown += gcnew System::Windows::Forms::KeyEventHandler(this, &Form1::Form1_KeyDown);
			this->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &Form1::Form1_KeyPress);
			this->KeyUp += gcnew System::Windows::Forms::KeyEventHandler(this, &Form1::Form1_KeyUp);
			this->groupBox1->ResumeLayout(false);
			this->groupBox1->PerformLayout();
			this->groupBox2->ResumeLayout(false);
			this->groupBox2->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericUpDown1))->EndInit();
			this->groupBox3->ResumeLayout(false);
			this->groupBox3->PerformLayout();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion

	private: System::Void button1_Click(System::Object^  sender, System::EventArgs^  e) {
				
			

			 }
	private: System::Void Form1_Load(System::Object^  sender, System::EventArgs^  e) {
			 }
	private: System::Void button2_Click(System::Object^  sender, System::EventArgs^  e) {
			
			 }
	private: System::Void button1_MouseUp(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
			testArm.armStop();
			 }
private: System::Void button2_MouseUp(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
			 testArm.armStop();
		 }
private: System::Void button1_MouseDown(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
			testArm.baseClockwise();
		 }
private: System::Void button3_Click(System::Object^  sender, System::EventArgs^  e) {
		 }
private: System::Void button3_MouseDown(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
			 testArm.shoulderUp();
		 }
private: System::Void button4_MouseDown(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
			 testArm.shoulderDown();
		 }
private: System::Void button5_MouseDown(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
			 testArm.elbowUp();
		 }
private: System::Void button6_MouseDown(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
			 testArm.elbowDown();
		 }
private: System::Void button7_MouseDown(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
			 testArm.wristUp();
		 }
private: System::Void button8_MouseDown(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
			 testArm.wristDown();
		 }
private: System::Void button9_MouseDown(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
			 testArm.gripperClose();
		 }
private: System::Void button10_MouseDown(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
			 testArm.gripperOpen();
		 }
private: System::Void button11_Click(System::Object^  sender, System::EventArgs^  e) {
			 testArm.searchlightOn();
		 }
private: System::Void button12_Click(System::Object^  sender, System::EventArgs^  e) {
			 testArm.searchlightOff();
		 }
private: System::Void button2_MouseDown(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
			 testArm.baseCounterClockwise();

		 }
private: System::Void button3_MouseUp(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
			 testArm.armStop();
		 }
private: System::Void button4_MouseUp(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
			 testArm.armStop();
		 }
private: System::Void button5_MouseUp(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
			 testArm.armStop();
		 }
private: System::Void button6_MouseUp(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
			 testArm.armStop();
		 }
private: System::Void button7_MouseUp(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
			 testArm.armStop();
		 }
private: System::Void button8_MouseUp(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
			 testArm.armStop();
		 }
private: System::Void button9_MouseUp(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
			 testArm.armStop();
		 }
private: System::Void button10_MouseUp(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
			 testArm.armStop();
		 }
private: System::Void button12_Click_1(System::Object^  sender, System::EventArgs^  e) {
			 testArm.searchlightOff();
		 }
private: System::Void Form1_KeyDown(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e) 
		{



		 }
private: System::Void button5_Click(System::Object^  sender, System::EventArgs^  e) {
		 }
private: System::Void Form1_KeyPress(System::Object^  sender, System::Windows::Forms::KeyPressEventArgs^  e) {
		
        
			 
		 }
private: System::Void Form1_KeyUp(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e) {

			testArm.armStop();
		 }
private: System::Void textBox1_TextChanged(System::Object^  sender, System::EventArgs^  e) {
			System::String ^ Up ;
			System::String ^ Down ;
			System::String ^ Neutral;
			System::String ^ Left;
			System::String ^ Right;
			
			Up = "U";
			Down = "D" ;
			Neutral = "N";
			Left = "L";
			Right = "R";

			if (Up == textBox1->Text )
			{
				testArm.elbowUp();
			}
			else if (Down == textBox1->Text )
			{
				testArm.elbowDown();
			
			}
			else if (Neutral == textBox1->Text )
			{
				testArm.armStop();
			
			}
			else if (Right == textBox1->Text )
			{
				testArm.baseClockwise();
			
			}
			else if (Left == textBox1->Text )
			{
				testArm.baseCounterClockwise();
			
			}
			Sleep(50);
			
			textBox1->Text = "" ;
		 }
private: System::Void textBox1_KeyPress(System::Object^  sender, System::Windows::Forms::KeyPressEventArgs^  e) {
		 }
private: System::Void textBox1_KeyDown(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e) {
			 if ( e->KeyCode == Keys::U )
            {
            testArm.searchlightOn();
            }
			 else if  ( e->KeyCode == Keys::D)
			{
				testArm.searchlightOff();

			 }
		 }
private: System::Void timer1_Tick(System::Object^  sender, System::EventArgs^  e) 		 
		 {
			 eegTxRxData receivedDataState;
	    HANDLE pipe = CreateNamedPipe(
		L"\\\\.\\pipe\\EEG_DATA_PIPE", // name of the pipe
		PIPE_ACCESS_INBOUND, // 1-way pipe -- RECEIVE only
		PIPE_TYPE_BYTE, // send data as a byte stream
		1, // only allow 1 instance of this pipe
		0, // no outbound buffer
		0, // no inbound buffer
		0, // use default wait time
		NULL // use default security attributes
	);

	if (pipe == NULL || pipe == INVALID_HANDLE_VALUE) {
	//	wcout << "Failed to create outbound pipe instance.";
		// look up error code here using GetLastError()
		//system("pause");
	//	return 1;
	}

//	wcout << "Waiting for a client to connect to the pipe..." << endl;

	// This call blocks until a client process connects to the pipe
	BOOL result = ConnectNamedPipe(pipe, NULL);
	if (!result) {
	//	wcout << "Failed to make connection on named pipe." << endl;
		// look up error code here using GetLastError()
		CloseHandle(pipe); // close the pipe
		//system("pause");
//		return 1;
	}
	else
	{

	//	wcout << "Reading Data from PIPE..." << endl;
		char data[1024]; //1024 is way too big and numBytesToRead2 is always 0 
		DWORD _numBytesRead = 0;
		BOOL	result = ReadFile(
		pipe,
		data, // the data from the pipe will be put here
		1024, // number of bytes allocated
		&_numBytesRead, // this will store number of bytes actually read
		NULL // not using overlapped IO
		);
	
		//cout << data;
	
		if (result) 
		{
			data[_numBytesRead] = '\0';// null terminate the string
			
			std::stringstream MyStringStream(data) ;
			boost::archive::text_iarchive ia(MyStringStream);
			
			ia >>receivedDataState ;

			
		
		} else {
	//		wcout << "Failed to read data from the pipe." << endl;
		}

		


	}
		CloseHandle(pipe);

//update form
		//IS Cognitive even active?
	if (receivedDataState.CognitivIsActive)
	{
		lblCognitivIsActive->Text = "YES!";
	}
	else
	{
		lblCognitivIsActive->Text = "No - Sit Still!";
	}

	//What is Cogntitiv Doing?
	if (receivedDataState.currentAction == COG_NEUTRAL)
	{
		lblCurrentAction->Text = "Neutral";
	}
	else if (receivedDataState.currentAction == COG_LIFT)
	{
		lblCurrentAction->Text = "Lift";
	}
	else if (receivedDataState.currentAction == COG_PUSH)
	{
		lblCurrentAction->Text = "Push";
	}
	else if (receivedDataState.currentAction == COG_PULL)
	{
		lblCurrentAction->Text = "Pull";
	}
	else if (receivedDataState.currentAction == COG_DROP)
	{
		lblCurrentAction->Text = "Drop";
	}
	else if (receivedDataState.currentAction == COG_LEFT)
	{
		lblCurrentAction->Text = "Left";
	}
	else
	{
		lblCurrentAction->Text = "I can't find it captain!";
	}

	//How powerfull is this?
	int currentActionPowerInt = (int) (receivedDataState.currentActionPower * 100);
	progressBarCogPower->Value = currentActionPowerInt ;

	//Headset Contact Qaulity
	// O1 
	if (receivedDataState.ContactQ_O1 == 	EEG_CQ_NO_SIGNAL)
	{
		lblCQO1->Text = "No Signal!" ;
	}
	else if (receivedDataState.ContactQ_O1 == EEG_CQ_VERY_BAD)
	{
		lblCQO1->Text = "Very Bad!" ;
	}
	else if (receivedDataState.ContactQ_O1 == EEG_CQ_POOR)
	{
		lblCQO1->Text = "Poor." ;
	}
	else if (receivedDataState.ContactQ_O1 == EEG_CQ_FAIR)
	{
		lblCQO1->Text = "Fair..." ;
	}
	else if (receivedDataState.ContactQ_O1 == EEG_CQ_GOOD)
	{
		lblCQO1->Text = "GOOD!" ;
	}
	else
	{
		lblCQO1->Text = "undefined signal Q?" ;
	}
	// O2
	if (receivedDataState.ContactQ_O2 == 	EEG_CQ_NO_SIGNAL)
	{
		lblCQO2->Text = "No Signal!" ;
	}
	else if (receivedDataState.ContactQ_O2 == EEG_CQ_VERY_BAD)
	{
		lblCQO2->Text = "Very Bad!" ;
	}
	else if (receivedDataState.ContactQ_O2 == EEG_CQ_POOR)
	{
		lblCQO2->Text = "Poor." ;
	}
	else if (receivedDataState.ContactQ_O2 == EEG_CQ_FAIR)
	{
		lblCQO2->Text = "Fair..." ;
	}
	else if (receivedDataState.ContactQ_O2 == EEG_CQ_GOOD)
	{
		lblCQO2->Text = "GOOD!" ;
	}
	else
	{
		lblCQO2->Text = "undefined signal Q?" ;
	}
		// P7
	if (receivedDataState.ContactQ_P7 == 	EEG_CQ_NO_SIGNAL)
	{
		lblCQP7->Text = "No Signal!" ;
	}
	else if (receivedDataState.ContactQ_P7 == EEG_CQ_VERY_BAD)
	{
		lblCQP7->Text = "Very Bad!" ;
	}
	else if (receivedDataState.ContactQ_P7 == EEG_CQ_POOR)
	{
		lblCQP7->Text = "Poor." ;
	}
	else if (receivedDataState.ContactQ_P7 == EEG_CQ_FAIR)
	{
		lblCQP7->Text = "Fair..." ;
	}
	else if (receivedDataState.ContactQ_P7 == EEG_CQ_GOOD)
	{
		lblCQP7->Text = "GOOD!" ;
	}
	else
	{
		lblCQP7->Text = "undefined signal Q?" ;
	}
		// P8
	if (receivedDataState.ContactQ_P8 == 	EEG_CQ_NO_SIGNAL)
	{
		lblCQP8->Text = "No Signal!" ;
	}
	else if (receivedDataState.ContactQ_P8 == EEG_CQ_VERY_BAD)
	{
		lblCQP8->Text = "Very Bad!" ;
	}
	else if (receivedDataState.ContactQ_P8 == EEG_CQ_POOR)
	{
		lblCQP8->Text = "Poor." ;
	}
	else if (receivedDataState.ContactQ_P8 == EEG_CQ_FAIR)
	{
		lblCQP8->Text = "Fair..." ;
	}
	else if (receivedDataState.ContactQ_P8 == EEG_CQ_GOOD)
	{
		lblCQP8->Text = "GOOD!" ;
	}
	else
	{
		lblCQP8->Text = "undefined signal Q?" ;
	}

	//Is the headset on correctly?
/*		Return whether the headset has been put on correctly or not. 
		If the headset cannot not be detected on the head, then signal quality will not report any results for all the channels
		Parameters:
		state  - EmoStatehandle 
		Returns:
		int - (1: On, 0: Off) 
*/
	if(receivedDataState.HeadSetOn != 0)
	{
		lblHeadSetOn->Text = "Yes!";
	}
	else
	{
		lblHeadSetOn->Text = "Nope, adjust it!";
	}
	
	


 

}
private: System::Void checkBox1_CheckedChanged(System::Object^  sender, System::EventArgs^  e) 
		 {
			 if  (checkBoxUsbEnabled-> Checked == true)
			 {
				if (!testArm.armConnect())
				{
					MessageBoxA(NULL,"The USB arm could not be connected!", "TUM Tum tummm!", MB_OK);
					checkBoxUsbEnabled-> Checked = false;
				}
				else
				{
					button1->Enabled = true;
					button2->Enabled = true;
					button3->Enabled = true;
					button4->Enabled = true;
					button5->Enabled = true;
					button6->Enabled = true;
					button7->Enabled = true;
					button8->Enabled = true;
					button9->Enabled = true;
					button10->Enabled = true;
					button11->Enabled = true;
					button12->Enabled = true;

				}
 
			 }
			 else
			 {
					testArm.armDisConnect();
					button1->Enabled = false;
					button2->Enabled = false;
					button3->Enabled = false;
					button4->Enabled = false;
					button5->Enabled = false;
					button6->Enabled = false;
					button7->Enabled = false;
					button8->Enabled = false;
					button9->Enabled = false;
					button10->Enabled = false;
					button11->Enabled = false;
					button12->Enabled = false;
			 }

		 }
private: System::Void button13_Click(System::Object^  sender, System::EventArgs^  e) {
			 

			 
		 }
private: System::Void groupBox2_Enter(System::Object^  sender, System::EventArgs^  e) {
		 }
private: System::Void chart1_Click(System::Object^  sender, System::EventArgs^  e) {
		 }
private: System::Void label6_Click(System::Object^  sender, System::EventArgs^  e) {
		 }
private: System::Void label9_Click(System::Object^  sender, System::EventArgs^  e) {
		 }
private: System::Void label8_Click(System::Object^  sender, System::EventArgs^  e) {
		 }
private: System::Void numericUpDown1_ValueChanged(System::Object^  sender, System::EventArgs^  e) {
		 }
};


}


